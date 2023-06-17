// When the user clicks on the extension action
chrome.action.onClicked.addListener( function() {
    console.log("Clicked Extension!")
    chrome.action.setPopup({popup: './popup/popup.html'})
});
