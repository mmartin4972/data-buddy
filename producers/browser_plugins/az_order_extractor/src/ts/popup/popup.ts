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


// INITIALIZERS
function add_event_listeners() {
    const button = document.getElementById("amazonButton");
    button?.addEventListener("click", az_button_clicked);
}


// EVENT HANDLERS
function az_button_clicked() {
    console.log("Button clicked!"); // Test if the event listener is working correctly
    // Send a message to the background service worker
    send_message_to_background({message: "Hello from the popup!"});
}


// MAIN
function init() {
    console.log("Popup window loaded!");
    background_port = chrome.runtime.connect({name: "popup"}); // Can specify extension ID, so only need to specify popup, inject, or background
    add_event_listeners(); // Make sure that cannot click button before connected to background
}
  
$(document).ready( () => init() );