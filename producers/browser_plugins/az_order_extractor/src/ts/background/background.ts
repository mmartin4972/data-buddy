// Main entry point for the background service worker, which is performing all processing, caching, and networking requests for the extension.
// Use Chrome ports to communicate with the injection script and the popup window

import * as service from "./service_api_lib";
import * as common from "../common";
let popup_port: chrome.runtime.Port;
let inject_port: chrome.runtime.Port;
let buddy: service.Buddy;
let client: service.Client;

// HELPERS
function background_log(...args: any[]) {
    console.log("[BACKGROUND] ")
    console.log(...args);
}

function handle_db_call(res: service.Response, success_callback: Function) {
    if (res.status_code === 200) {
        success_callback();
    } else {
        background_log("Error: ", res);
        let error: string = res.data["error"];
        popup_port.postMessage({type: common.RespPopupMessageType.DisplayError, data: {error: error}});
    }
}

function isJSON(value:any) {
    try {
        JSON.parse(value);
        return true;
    } catch (error) {
        return false;
    }
}

// EVENT HANDLERS
async function recv_popup_message(message: common.Message) {
    switch (message.type) {
        case common.RecvPopupMessageType.Create:
            background_log("Got create message")
            handle_db_call (await buddy.create(message.data["path"]),
                () => {popup_port.postMessage({type:common.RespPopupMessageType.StateChange, data: {state: common.PopupState.ConnectedWait}})});
            break;
        case common.RecvPopupMessageType.Connect:
            background_log("Got connect message")
            handle_db_call( await buddy.connect(message.data["path"]), 
                () => {popup_port.postMessage({type:common.RespPopupMessageType.StateChange, data: {state: common.PopupState.ConnectedWait}})});
            
            break;
        case common.RecvPopupMessageType.Disconnect:
            background_log("Got disconnect message")
            handle_db_call(await buddy.disconnect(), 
                () => {popup_port.postMessage({type:common.RespPopupMessageType.StateChange, data: {state: common.PopupState.Disconnected}})});
        default:
            background_log("Unknown message type: ", message);
            break;
    }
}

function recv_inject_message(message: common.Message) {
    switch (message.type) {
        case common.RecvInjectMessageType.Put:
            break;
        case common.RecvInjectMessageType.RangePut: // maybe unnecessary
            break;
        default:
            background_log("Unknown message type: ", message);
            break;
    }
}

function init() {
    // Listen for messages from the popup window and injection script
    chrome.runtime.onConnect.addListener(function(newPort: chrome.runtime.Port) {
        if (newPort.name === common.Port.Popup) { // TODO: maybe have some common file and make this an enum
            popup_port = newPort;
            popup_port.onMessage.addListener(recv_popup_message);
        } else if (newPort.name === common.Port.Inject) {
            inject_port = newPort;
            inject_port.onMessage.addListener(recv_inject_message);
        } else {
            background_log("Unknown port: ", newPort);
        }
    })
    background_log("Connecting to buddy");
    buddy = new service.Buddy(common.BUDDY_URL);
}

init();
