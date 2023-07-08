## Installation
The extension is (finally) listed on the chrome-web-store at [WEB_NAV_EXTENSION](https://chrome.google.com/webstore/detail/web-nav-extension/cmpdibdfnkjolimghiflbengofjboobl?hl=en&authuser=0)

To install and use locally:
1. Clone repo into a desired directory using `git clone https://github.com/Feel-ix-343/Web_Nav.git`
2. Navigate to the *webnav_analysis* directory, and run `wasm-pack build --release --target web` in the command prompt. This will build the rust wasm code and produce an npm package in the *webnav_analysis/pkg* directory
    - Note, you will need to have rust and [wasm-pack](https://github.com/rustwasm/wasm-pack) installed
3. Navigate to the *extension* directory, and run `npm install` to install dependencies. Then run `npm run build` to produce the extension files in the *extension/dist* directory. If you want to load on file change, run `npm run watch`
4. Open chrome://extensions and turn on developer mode
5. "Load Unpack" the *./extension/dist/* directory and open the extension
