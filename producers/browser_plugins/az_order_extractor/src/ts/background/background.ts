// Main entry point for the background service worker, which is performing all processing, caching, and networking requests for the extension.
// Use Chrome ports to communicate with the injection script and the popup window

var popup_port: chrome.runtime.Port;
var inject_port: chrome.runtime.Port;

// Listen for messages from the popup window
function init() {
    // Listen for messages from the popup window and injection script
    chrome.runtime.onConnect.addListener(function(newPort: chrome.runtime.Port) {
        if (newPort.name === "popup") {
            popup_port = newPort;
            console.log("Popup connected!");
            popup_port.onMessage.addListener(function(message: any) {
                console.log("Message from popup:", message);
            });
            popup_port.postMessage({message: "Hello from the background!"});
        } else if (newPort.name === "inject") {
            inject_port = newPort;
            console.log("Injection script connected!");
            inject_port.onMessage.addListener(function(message: any) {
                console.log("Message from injection script:", message);
            });
            inject_port.postMessage({message: "Hello from the background!"});
        }
    })
}

init();
