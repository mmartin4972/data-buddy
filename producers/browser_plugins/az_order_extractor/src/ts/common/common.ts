
export enum Port {
    Popup = "popup",
    Inject = "inject",
    Background = "background"
}

export enum MessageType {
    CreateDb = "create_db",
}

export interface Message {
    type: MessageType;
    data: any;
}

