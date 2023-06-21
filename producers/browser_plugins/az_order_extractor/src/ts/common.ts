export const BUDDY_URL = "http://localhost:8787"

export enum Port {
    Popup = "popup",
    Inject = "inject",
    Background = "background"
}

export enum RecvInjectMessage {
    Put = "put",
    RangePut = "range_put"
}

export enum RecvPopupMessage {
    Create = "create_db",
    Connect = "connect_db",
    Disconnect = "disconnect_db"
}

export interface Message {
    type: string;
    data: any;
}

