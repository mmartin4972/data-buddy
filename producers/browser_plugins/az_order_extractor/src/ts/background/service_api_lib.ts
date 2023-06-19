function post_json(url: string, data: any) {
    return fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data),
    });
}

export class Client {
    readonly name: string;
    readonly password: string;
    auth_token: string;
    url: string;

    constructor(name: string, password: string, auth_token?: string) {
        this.name = name;
        this.password = password;
        if (typeof auth_token !== 'undefined') {
            this.auth_token = auth_token;
        } else {
            this.auth_token = '';
        }
        this.url = '';
    }


}

export class Buddy {
    url: string;
    path: string;

    constructor(url: string, path?: string) {
        this.url = url;
        if (typeof path !== 'undefined') {
            this.path = path;
        } else {
            this.path = '';
        }
    }

    async create(path: string) {
        let data: any = {
            'path': path,
        }
        let res = await post_json(this.url + '/db-create-buddy', data);
        if (res.ok) {
            let json = await res.json();
            this.path = json['path'];
        }
        return res;
    }
}