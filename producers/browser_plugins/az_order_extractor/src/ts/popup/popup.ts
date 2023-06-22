// Entrypoint for all code that runs in the popup window.
// Use Chrome ports to communicate with the background service worker and the injection script
'use strict';

const $ = require('jquery');
import * as common from '../common';

// GLOBALS
let background_port: chrome.runtime.Port;
const scrape_btn = document.getElementById("scrape_btn")!;
const connect_form = document.getElementById("connect_form")!;
const create_form = document.getElementById("create_form")!;
const disconnect_btn = document.getElementById("disconnect_btn")!;
const status_icon = document.getElementById("status_icon")!;
const order_count = document.getElementById("order_count")!;
const order_line = document.getElementById("order_line")!;
const error_display = document.getElementById("error_display")!;


// HELPERS
function send_message_to_background(message: common.Message) {
    background_port.postMessage(message);
}

function popup_log(...args: any[]) {
    console.log("[POPUP]")
    console.log(...args);
}

// State Transition Functions

function enter_disconnected_state() {
    status_icon.classList.remove("connected");
    status_icon.classList.add("disconnected");    
    disconnect_btn.style.display = "none";
    scrape_btn.style.display = "none";
    create_form.style.display = "block";
    connect_form.style.display = "block";
    order_line.style.display = "none";
    order_count.style.display = "none";
    error_display.style.display = "none"; 
}

function enter_connected_wait() {
    status_icon.classList.remove("disconnected");
    status_icon.classList.add("connected");
    disconnect_btn.style.display = "block";
    scrape_btn.style.display = "block";
    create_form.style.display = "none";
    connect_form.style.display = "none";
    order_line.style.display = "none";
    order_count.style.display = "none";
    error_display.style.display = "none";
}

function enter_connected_scrape_state() {
    status_icon.classList.remove("disconnected");
    status_icon.classList.add("connected");
    disconnect_btn.style.display = "block";
    scrape_btn.style.display = "block";
    create_form.style.display = "none";
    connect_form.style.display = "none";
    order_line.style.display = "block";
    order_count.style.display = "block";
    error_display.style.display = "none";
}

// EVENT HANDLERS
function recv_background_message(message: common.Message) {
    switch (message.type) {
        case common.RespPopupMessageType.StateChange:
            switch(message.data["state"]) {
                case common.PopupState.Disconnected:
                    enter_disconnected_state();
                    break;
                case common.PopupState.ConnectedWait:
                    enter_connected_wait();
                    break;
                case common.PopupState.ConnectedScrape:
                    enter_connected_scrape_state();
                    break;
            }
            break;
        case common.RespPopupMessageType.DisplayError:
            error_display.style.display = "block";
            error_display.innerText = message.data["error"];
            break;
        case common.RespPopupMessageType.OrderCountUpdate:
            order_count.innerText = message.data["count"];
            break;
        default:
            popup_log("Unknown message type from background: ", message);
            break;
    }
}

async function form_submitted(event: Event) {
    popup_log(event)
    event.preventDefault(); // Prevent the form from submitting
    let triggered_id = (event.target as HTMLFormElement)!.id;
    if (triggered_id === "connect_form") {
        let path = (document.getElementById("connect_path") as HTMLInputElement)!.value;
        send_message_to_background({type: common.RecvPopupMessageType.Connect, data: {path: path}});
    } else if (triggered_id === "create_form") {
        let path = (document.getElementById("create_path") as HTMLInputElement)!.value;
        send_message_to_background({type: common.RecvPopupMessageType.Create, data: {path: path}});
    }
}

async function disconnect_btn_clicked() {
    send_message_to_background({type: common.RecvPopupMessageType.Disconnect, data: {}}); // TODO: maybe error handling?
}

async function scrape_btn_clicked() {
    send_message_to_background({type: common.RecvPopupMessageType.Scrape, data: {}});
}

// MAIN
function init() {
    popup_log("Popup window loaded!");
    enter_disconnected_state();
    background_port = chrome.runtime.connect({name: "popup"}); // Can specify extension ID, so only need to specify popup, inject, or background
    background_port.onMessage.addListener(recv_background_message);
    create_form?.addEventListener("submit", form_submitted);
    connect_form?.addEventListener("submit", form_submitted);
    disconnect_btn?.addEventListener("click", disconnect_btn_clicked);
    scrape_btn?.addEventListener("click", scrape_btn_clicked);
}
  
$(document).ready( () => init() );