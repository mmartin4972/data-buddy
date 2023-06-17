// export class Client {
//   constructor(name = "", password = "", auth_token = "") {
//     this.name = name;
//     this.password = password;
//     this.auth_token = auth_token;
//     this.url = "";
//   }

//   get(category, key) {
//     var data;
//     data = {
//       "name": this.name,
//       "auth_token": this.auth_token,
//       "category_name": category,
//       "key": json.dumps(key)
//     };
//     return requests.post(this.url + "/db-get", {
//       "json": data
//     });
//   }

//   get_range(category, key) {
//     var data;
//     data = {
//       "name": this.name,
//       "auth_token": this.auth_token,
//       "category_name": category,
//       "key": json.dumps(key)
//     };
//     return requests.post(this.url + "/db-get-range", {
//       "json": data
//     });
//   }

//   put(category, key, value) {
//     var data;
//     data = {
//       "name": this.name,
//       "auth_token": this.auth_token,
//       "category_name": category,
//       "key": json.dumps(key),
//       "value": json.dumps(value)
//     };
//     return requests.post(this.url + "/db-put", {
//       "json": data
//     });
//   }

//   create_category(category, key_schema, value_schema) {
//     var data;
//     data = {
//       "name": this.name,
//       "auth_token": this.auth_token,
//       "category_name": category,
//       "key_schema": json.dumps(key_schema),
//       "value_schema": json.dumps(value_schema)
//     };
//     return requests.post(this.url + "/db-create-category", {
//       "json": data
//     });
//   }

//   add_client_to_category(category, add_client) {
//     var data;
//     data = {
//       "name": this.name,
//       "auth_token": this.auth_token,
//       "category_name": category,
//       "add_name": add_client.name
//     };
//     return requests.post(this.url + "/db-add-client", {
//       "json": data
//     });
//   }

// }

export class Buddy {
  constructor(url, path = "") {
    this.path = path;
    this.url = url;
  }

  async create(path) {
    var res;
    var data = {
        "path": path
      }
    res = await fetch(this.url, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json'
        },
        body: JSON.stringify(data)
    })
    if (res.status_code === 200) {
        res_parsed = await res.json()
        this.path = res["folder_path"];
    }

    return res;
  }

//   connect() {
//     return requests.post(this.url + "/db-connect-buddy", {
//       "json": {
//         "path": this.path
//       }
//     });
//   }

  disconnect() {
    return fetch(this.url + "/db-disconnect-buddy");
  }

//   create_client(client) {
//     var data, res;
//     data = {
//       "name": client.name,
//       "password": client.password
//     };
//     res = requests.post(this.url + "/db-create-client", {
//       "json": data
//     });

//     if (res.status_code === 200) {
//       client.url = this.url;
//       client.auth_token = res.json()["auth_token"];
//     }

//     return res;
//   }

//   connect_client(client) {
//     var data, res;
//     data = {
//       "name": client.name,
//       "password": client.password
//     };
//     res = requests.post(this.url + "/db-connect-client", {
//       "json": data
//     });

//     if (res.status_code === 200) {
//       client.url = this.url;
//       client.auth_token = res.json()["auth_token"];
//     }

//     return res;
//   }

//   disconnect_client(client) {
//     var data;
//     data = {
//       "name": client.name,
//       "auth_token": client.auth_token
//     };
//     return requests.post(this.url + "/db-disconnect-client", {
//       "json": data
//     });
//   }

  async list_clients() {
    fetch(this.url + "/db-list-clients")
    .then(response => response.text())
    .then(body => {
      const data = JSON.parse(body);
      console.log(data);
      console.log(data.propertyName);
    })
    .catch(error => {
      console.error('Error:', error);
    });;
  }

//   list_categories() {
//     return requests.get(this.url + "/db-list-categories");
//   }

  get_path() {
    return this.path;
  }

  get_url() {
    return this.url;
  }

}
