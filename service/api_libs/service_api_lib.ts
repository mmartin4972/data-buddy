async function handle_response(res: any): Promise<Response> {
    let response: Response = new Response(500, {});
    await res.text()
    .then(function (parsed: any) {
        let json: any;
        try {
            json = JSON.parse(parsed);
        } catch (error) {
            throw new Error("Trouble parsing following as JSON. " + parsed);
        }
        response = new Response(res.status, json);
    })
    .catch(function (err: any) {
        response = new Response(res.status, {"error": "Trouble parsing as text. " + err});
    });
    return response;
}

async function post_json(url: string, data: any): Promise<Response> {
    return await fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data),
    })
    .then(handle_response)
    .catch(function (err: any) {
        return new Response(500, {"error": "Trouble fetching. " + err});
    });
}

async function get_json(url: string): Promise<Response> {
    return await fetch(url, {
        method: 'GET'
    })
    .then(handle_response)
    .catch(function (err: any) {
        return new Response(500, err);
    });
}

export class Response {
    status_code: number;
    data: any;

    constructor(status_code: number, data: any) {
        this.status_code = status_code;
        this.data = data;
    }
}

export class Client {
    readonly name: string;
    readonly password: string;
    auth_token: string;
    url: string;

    constructor(name: string, password: string, auth_token?: string, url?: string) {
        this.name = name;
        this.password = password;
        if (typeof auth_token !== 'undefined') {
            this.auth_token = auth_token;
        } else {
            this.auth_token = '';
        }
        if (typeof url !== 'undefined') {
            this.url = url;
        } else {
            this.url = '';
        }
    }

    async is_authenticated() : Promise<Response> {
        if (this.url === '' || this.auth_token === '') {
            return new Response(200, {"data": false, "error": ""});
        }
        let data: any = {
            'name': this.name,
            'auth_token': this.auth_token
        }
        return await post_json(this.url + '/db-check-authenticated', data);
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

    async create(path: string): Promise<Response> {
        let data: any = {
            'path': path
        }
        let res = await post_json(this.url + '/db-create-buddy', data);
        if (res.status_code === 200) {
            this.path = res.data['path'];
        }
        return res;
    }

    async connect(path?: string): Promise<Response> {
        let p = this.path;
        if (typeof path !== 'undefined') {
            p = path;
        }
        let data: any = {
            'path': p
        }
        return await post_json(this.url + '/db-connect-buddy', data);
    }

    async disconnect(): Promise<Response> {
        return await get_json(this.url + '/db-disconnect-buddy');
    }

    async create_client(client: Client): Promise<Response> {
        let data: any = {
            'name': client.name,
            'password': client.password
        }
        let res = await post_json(this.url + '/db-create-client', data);
        if (res.status_code === 200) {
            client.url = this.url;
            client.auth_token = res.data['auth_token'];
        }
        return res;
    }

    async connect_client(client: Client): Promise<Response> {
        let data: any = {
            'name': client.name,
            'password': client.password
        }
        let res = await post_json(this.url + '/db-connect-client', data);
        if (res.status_code === 200) {
            client.url = this.url;
            client.auth_token = res.data['auth_token'];
        }
        return res;
    }

    async disconnect_client(client: Client): Promise<Response> {
        let data: any = {
            'name': client.name,
            'auth_token': client.auth_token
        }
        return await post_json(this.url + '/db-disconnect-client', data);
    }

    async list_clients(): Promise<Response> {
        return await get_json(this.url + '/db-list-clients');
    }

    async list_categories(): Promise<Response> {
        return await get_json(this.url + '/db-list-categories');
    }

    get_path(): string {
        return this.path;
    }

    get_url(): string {
        return this.url;
    }

    async ping() : Promise<Response> {
        return await get_json(this.url + '/db-ping');
    }

    async is_connected(): Promise<Response> {
        return await get_json(this.url + '/db-check-buddy-connected');
    }
}