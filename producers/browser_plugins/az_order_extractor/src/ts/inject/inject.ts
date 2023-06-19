// Main entry point for all javascript code that interfaces with the DOM of the current page.
// Use Chrome ports to communicate with the background service worker and the popup window

import * as azad_inject from './azad/inject';

var background_port: chrome.runtime.Port;

// HELPERS
function inject_log(...args: any[]) {
    console.log("[INJECT]")
    console.log(...args);
}

// MAIN
function init() {
    console.log("Injection loaded!");
    background_port = chrome.runtime.connect({name: "inject"}); // Can specify extension ID, so only need to specify popup, inject, or background
}
  
init();