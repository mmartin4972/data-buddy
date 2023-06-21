// Entrypoint for all code that runs in the popup window.
// Use Chrome ports to communicate with the background service worker and the injection script
'use strict';

const $ = require('jquery');
const common = require('../common');

// GLOBALS
let background_port: chrome.runtime.Port;
const scrape_btn = document.getElementById("scrape_btn");
const connect_form = document.getElementById("connect_form");
const create_form = document.getElementById("create_form");
const disconnect_btn = document.getElementById("disconnect_btn");
const status_icon = document.getElementById("status_icon")!;


// HELPERS
function send_message_to_background(message: any) {
    background_port.postMessage(message);
}

function popup_log(...args: any[]) {
    console.log("[POPUP]")
    console.log(...args);
}

function set_status(connected: boolean) {
    if (connected) {
        status_icon.classList.remove("disconnected");
        status_icon.classList.add("connected");
        disconnect_btn!.style.display = "block";
        scrape_btn!.style.display = "block";
        create_form!.style.display = "none";
        connect_form!.style.display = "none";
    } else {
        status_icon.classList.remove("connected");
        status_icon.classList.add("disconnected");    
        disconnect_btn!.style.display = "none";
        scrape_btn!.style.display = "none";
        create_form!.style.display = "block";
        connect_form!.style.display = "block";
    }
}


// INITIALIZERS
function add_event_listeners() {
    create_form?.addEventListener("submit", form_submitted);
    connect_form?.addEventListener("submit", form_submitted);
    disconnect_btn?.addEventListener("click", disconnect_btn_clicked);
    scrape_btn?.addEventListener("click", scrape_btn_clicked);
    background_port.onMessage.addListener(recv_background_message);
}


// EVENT HANDLERS
function scrape_btn_clicked() {
    popup_log("Button clicked!"); // Test if the event listener is working correctly
    // Send a message to the background service worker
    send_message_to_background({message: "Hello from the popup!"});
}

function recv_background_message(message: any) {
    popup_log("Message from background:", message);
}

function form_submitted(event: Event) {
    popup_log(event)
    event.preventDefault(); // Prevent the form from submitting
    let triggered_id = (event.target as HTMLFormElement)!.id;
    let path = (document.getElementById(triggered_id) as HTMLInputElement)!.value;

    if (triggered_id === "connect_form") {
        popup_log("Connect submitted");
        // TODO: Error handling
        send_message_to_background({type: common.RecvPopupMessage.Connect, data: {path: path}});
    } else if (triggered_id === "create_form") {
        send_message_to_background({type: common.RecvPopupMessage.Create, data: {path: path}});
    } else {
        popup_log("Unknown form submitted: ", triggered_id);
    }

    set_status(true);
}

function disconnect_btn_clicked() {
    set_status(false);
    send_message_to_background({type: common.RecvPopupMessage.Disconnect, data: {}}); // TODO: maybe error handling?
}


// MAIN
function init() {
    popup_log("Popup window loaded!");
    set_status(false);
    background_port = chrome.runtime.connect({name: "popup"}); // Can specify extension ID, so only need to specify popup, inject, or background
    add_event_listeners(); // Make sure that cannot click button before connected to background
}
  
$(document).ready( () => init() );