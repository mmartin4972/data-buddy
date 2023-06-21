export const BUDDY_URL = "http://localhost:8787"

export enum PopupState {
    Disconnected = "disconnected",
    ConnectedWait = "connected_wait",
    ConnectedScrape = "connected_scraping"
}

export enum Port {
    Popup = "popup",
    Inject = "inject",
    Background = "background"
}

export enum RecvInjectMessageType {
    Put = "put",
    RangePut = "range_put"
}

export enum RecvPopupMessageType {
    Create = "create_db",
    Connect = "connect_db",
    Disconnect = "disconnect_db",
    Scrape = "scrape_db"
}

export enum RespPopupMessageType {
    StateChange = "state_change",
    OrderCountUpdate = "order_count_update",
    DisplayError = "display_error"
}

export interface Message {
    type: string;
    data: any;
}

