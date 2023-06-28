import requests
import json
from typing import TypeVar

def raise_error(e, res):
    raise Exception(str(e) + res.json()['error'])

# Class that is used for anything that requires authorization
# Client can only be connected to one database at a time
T = TypeVar('T', bound='Client')
class Client:
    def __init__(self, name="", password="", auth_token="", url=""):
        self.name = name
        self.password = password
        self.auth_token = auth_token
        self.url = url
        
    def get(self, category, key) :
        data = {
            'name': self.name,
            'auth_token': self.auth_token,
            'category_name': category,
            'key': json.dumps(key)
        }
        return requests.post(self.url + '/db-get', json=data)
    
    def get_range(self, category, key) :
        data = {
            'name': self.name,
            'auth_token': self.auth_token,
            'category_name': category,
            'key': json.dumps(key)
        }
        return requests.post(self.url + '/db-get-range', json=data)
    
    def put(self, category, key, value) :
        data = {
            'name': self.name,
            'auth_token': self.auth_token,
            'category_name': category,
            'key': json.dumps(key),
            'value': json.dumps(value)
        }
        return requests.post(self.url + '/db-put', json=data)
    
    def create_category(self, category: str, key_schema: dict, value_schema: dict) :
        data = {
            'name': self.name,
            'auth_token': self.auth_token,
            'category_name': category,
            'key_schema': json.dumps(key_schema),
            'value_schema': json.dumps(value_schema)
        }
        return requests.post(self.url + '/db-create-category', json=data)
    
    def add_client_to_category(self, category: str, add_client: T) :
        data = {
            'name': self.name,
            'auth_token': self.auth_token,
            'category_name': category,
            'add_name': add_client.name
        }
        return requests.post(self.url + '/db-add-client', json=data)
    
    def is_authenticated(self):
        data = {
            'name': self.name,
            'auth_token': self.auth_token
        }
        return requests.post(self.url + '/db-check-authenticated', json=data)

# Class that is used for database wide operations that don't require authorization
class Buddy:
    def __init__(self, url: str, path: str = ""):
        self.path = path
        self.url = url
    
    def create(self, path: str):
        res = requests.post(self.url + '/db-create-buddy', json={"path": path})
        if res.status_code == 200 :
            self.path = res.json()['folder_path']
        return res
            
    def connect(self, path: str = ""):
        p = self.path
        if path != "":
            p = path
        return requests.post(self.url + '/db-connect-buddy', json={"path": p})

    def disconnect(self):
        return requests.get(self.url + '/db-disconnect-buddy')
            
        
    def create_client(self, client: Client):
        data = {
            'name': client.name, 
            'password': client.password
        }
        res = requests.post(self.url + '/db-create-client', json=data)
        if (res.status_code == 200):
            client.url = self.url
            client.auth_token = res.json()['auth_token']
        return res          
    
    def connect_client(self, client: Client):
        data = {
            'name': client.name, 
            'password': client.password
        }
        res = requests.post(self.url + '/db-connect-client', json=data)
        if (res.status_code == 200):
            client.url = self.url
            client.auth_token = res.json()['auth_token']
        return res
    
    def disconnect_client(self, client: Client):
        data = {
            'name': client.name, 
            'auth_token': client.auth_token
        }
        return requests.post(self.url + '/db-disconnect-client', json=data)
    
    def list_clients(self):
        return requests.get(self.url + '/db-list-clients')
                
    def list_categories(self):
        return requests.get(self.url + '/db-list-categories')
            
    def get_path(self):
        return self.path
    
    def get_url(self):
        return self.url
    
    def ping(self):
        return requests.get(self.url + '/db-ping')
    
    def is_connected(self):
        return requests.get(self.url + '/db-check-buddy-connected')
    
    
    
