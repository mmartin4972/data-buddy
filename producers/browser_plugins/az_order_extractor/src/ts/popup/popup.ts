// Entrypoint for all code that runs in the popup window.
// Use Chrome ports to communicate with the background service worker and the injection script

// import * as db_service from "./service-js-api-lib.js";
// import * as azad_inject from "../azad/inject.js";

// document.addEventListener("DOMContentLoaded", function() {
//     var button = document.getElementById("amazonButton");
//     button.addEventListener("click", function() {
//         console.log("Button clicked!"); // Test if the event listener is working correctly
        
//         // Launch the network request to the data buddy server
//         console.log("Creating buddy")
//         let b = new service.Buddy("http://localhost:8787")
//         // let res = b.create("/test")
//         b.list_clients();
//     });
//   });