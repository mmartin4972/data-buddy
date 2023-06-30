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
    // May be an issue here in that we think that we are guaranteed to get a response
    if (res.status_code === 200) {
        success_callback();
    } else {
        background_log("Error: ", res);
        let error: string = res.data["error"];
        tell_popup_error(error);
    }
}

function tell_popup_error(error: string) {
    background_log("Error: ", error);
    popup_port.postMessage({type: common.RespPopupMessageType.DisplayError, data: {error: error}});
}

function tell_popup_connected() {
    popup_port.postMessage({type:common.RespPopupMessageType.StateChange, data: {state: common.PopupState.ConnectedWait}});
}

// TODO: Made this read a value from local memory or something
function create_db_client() {
    client = new service.Client("az_order_extractor", "super_secret_password");
}

// EVENT HANDLERS
async function recv_popup_message(message: common.Message): Promise<void> {
    switch (message.type) {
        case common.RecvPopupMessageType.Create:
            background_log("Got create message")
            handle_db_call (await buddy.create(message.data["path"]),
                async () => {popup_port.postMessage({type:common.RespPopupMessageType.StateChange, data: {state: common.PopupState.ConnectedWait}})});
            break;
        case common.RecvPopupMessageType.Connect:
            background_log("Got connect message")
            handle_db_call(await buddy.connect(message.data["path"]), 
                async (): Promise<void> => {
                    let res = await buddy.is_client_connected(client);
                    let res = await buddy.connect_client(client);
                    if (res.status_code === 200) {
                        background_log("Connected no createp all Good")
                        tell_popup_connected();
                    } else { // If get an error try to create the client
                        background_log("Error connecting, trying to create client", res.data["error"])
                        await buddy.create_client(client)
                        .then((res2 : any) => {
                            if (res2.status_code === 200) {
                                background_log("Created client, trying to connect again")
                                tell_popup_connected();
                            } else {
                                throw new Error(res2.data["error"]);
                            }
                        })
                        .catch((err: any) => {
                            tell_popup_error(err);
                        });
                    }
                }); /// TODO: Find a better way to structure this please!!
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
    create_db_client();
}

init();

// Currently concerned by how you can create a database that already exsists
// Also how you cannot connect to a database that is already connected