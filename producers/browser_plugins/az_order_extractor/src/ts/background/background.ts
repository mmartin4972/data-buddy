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

// Listen for messages from the popup window
function init() {
    // Listen for messages from the popup window and injection script
    background_log("Adding listeners");
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

// EVENT HANDLERS
async function recv_popup_message(message: common.Message) {
    switch (message.type) {
        case common.RecvPopupMessage.Create:
            background_log("create from popup: ", await buddy.create(message.data["path"]))
            break;
        case common.RecvPopupMessage.Connect:
            background_log("connect from popup: ", await buddy.connect(message.data["path"]));
            break;
        case common.RecvPopupMessage.Disconnect:
            background_log("disconnect from popup: ", await buddy.disconnect());
            break;
        default:
            background_log("Unknown message type: ", message);
            break;
    }
}

function recv_inject_message(message: common.Message) {
    switch (message.type) {
        case common.RecvInjectMessage.Put:
            break;
        case common.RecvInjectMessage.RangePut: // maybe unnecessary
            break;
        default:
            background_log("Unknown message type: ", message);
            break;
    }
}

init();
