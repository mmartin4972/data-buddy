async function post_json(url: string, data: any): Promise<Response> {
    let response: Response = new Response(500, {});
    await fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data),
    })
    .then(async function (res: any) {
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
    })
    .catch(function (err: any) {
        response = new Response(500, {"error": "Trouble fetching. " + err});
    });
    return response;
}

async function get_json(url: string): Promise<Response> {
    let response: Response = new Response(500, {});
    await fetch(url, {
        method: 'GET'
    })
    .then(async function (res: any) {
        await res.json()
        .then(function (json: any) {
            response = new Response(res.status, json);
        })
        .catch(function (err: any) {
            response = new Response(res.status, err);
        });
    })
    .catch(function (err: any) {
        response = new Response(500, err);
    });
    return response;
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

export class Response {
    status_code: number;
    data: any;

    constructor(status_code: number, data: any) {
        this.status_code = status_code;
        this.data = data;
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
}