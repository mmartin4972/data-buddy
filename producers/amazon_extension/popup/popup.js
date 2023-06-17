import * as service from "./service-js-api-lib.js"; // path relative to this file

document.addEventListener("DOMContentLoaded", function() {
    var button = document.getElementById("amazonButton");
    button.addEventListener("click", function() {
        console.log("Button clicked!"); // Test if the event listener is working correctly
        
        // Launch the network request to the data buddy server
        console.log("Creating buddy")
        let b = new service.Buddy("http://localhost:8787")
        // let res = b.create("/test")
        b.list_clients();
    });
  });
