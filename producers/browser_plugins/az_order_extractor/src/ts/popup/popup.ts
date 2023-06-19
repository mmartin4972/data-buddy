// Entrypoint for all code that runs in the popup window.
// Use Chrome ports to communicate with the background service worker and the injection script
'use strict';

const $ = require('jquery');

// GLOBALS
var background_port: chrome.runtime.Port;

// HELPERS
function send_message_to_background(message: any) {
    background_port.postMessage(message);
}

function popup_log(...args: any[]) {
    console.log("[POPUP]")
    console.log(...args);
}


// INITIALIZERS
function add_event_listeners() {
    const button = document.getElementById("amazonButton");
    button?.addEventListener("click", az_button_clicked);
    background_port.onMessage.addListener(recv_background_message);
}


// EVENT HANDLERS
function az_button_clicked() {
    popup_log("Button clicked!"); // Test if the event listener is working correctly
    // Send a message to the background service worker
    send_message_to_background({message: "Hello from the popup!"});
}

function recv_background_message(message: any) {
    popup_log("Message from background:", message);
}


// MAIN
function init() {
    popup_log("Popup window loaded!");
    background_port = chrome.runtime.connect({name: "popup"}); // Can specify extension ID, so only need to specify popup, inject, or background
    add_event_listeners(); // Make sure that cannot click button before connected to background
}
  
$(document).ready( () => init() );