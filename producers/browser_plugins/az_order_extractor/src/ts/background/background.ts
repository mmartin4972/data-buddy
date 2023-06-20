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
    chrome.runtime.onConnect.addListener(function(newPort: chrome.runtime.Port) {
        if (newPort.name === common.Port.Popup) { // TODO: maybe have some common file and make this an enum
            popup_port = newPort;
            background_log("Popup connected!");
            popup_port.onMessage.addListener(recv_popup_message);
            popup_port.postMessage({message: "Hello from the background!"});
        } else if (newPort.name === "inject") {
            inject_port = newPort;
            background_log("Injection script connected!");
            inject_port.onMessage.addListener(function(message: any) {
                background_log("Message from injection script:" + String(message));
            });
            inject_port.postMessage({message: "Hello from the background!"});
        }
    })
}

// EVENT HANDLERS
function recv_popup_message(message: any) {
    switch (message.type) {
        case "create_db":
            buddy = new service.Buddy("http://localhost:8787");
            break;
        default:
            background_log("Unknown message type: " + String(message.type));
            break;
    }
}

init();
