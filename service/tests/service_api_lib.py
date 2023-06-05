
import requests

def raise_error(e, res):
    raise Exception(str(e) + res.json()['error'])

# Class that is used for anything that requires authorization
# Client can only be connected to one database at a time
class Client:
    def __init__(self, name="", password="", auth_token=""):
        self.name = name
        self.password = password
        self.auth_token = auth_token
        
    def get(category, key, prefix_end="") :
        return
    
    def put(category, key, value, prefix_end="") :
        return
    
    def create_category(category, key_schema, value_schema) :
        return
    
    def add_client_to_catetgory(category, client) :
        return

# Class that is used for database wide operations that don't require authorization
class Buddy:
    def __init__(self, url, path=""):
        self.path = path
        self.url = url
    
    def create(self, path):
        try:
            res = requests.post(self.url + '/db-create-buddy', json={"path": path})
            assert(res.status_code == 200)
            self.path = res.json()['folder_path']
        except Exception as e :
            raise_error(e, res)
        return res
            
            
    def connect(self):
        try:
            res = requests.post(self.url + '/db-connect-buddy', json={"path": self.path})
            assert(res.status_code == 200)
        except Exception as e :
            raise_error(e, res)
        return res
            
    
    def disconnect(self):
        try:
            res = requests.get(self.url + '/db-disconnect-buddy')
            assert(res.status_code == 200)
        except Exception as e :
            raise_error(e, res)
        return res
            
        
    def create_client(self, client: Client):
        try:
            data = {'name': client.name, 
                    'password': client.password
                    }
            res = requests.post(self.url + '/db-create-client', json=data)
            assert(res.status_code == 200)
        except Exception as e :
            raise_error(e, res)
        return res
            
    
    def connect_client(self, client: Client):
        try:
            data = {'name': client.name, 
                    'password': client.password
                    }
            res = requests.post(self.url + '/db-connect-client', json=data)
            assert(res.status_code == 200)
        except Exception as e :
            raise_error(e, res)
        return res
    
    def disconnect_client(self, client: Client):
        try:
            data = {'name': client.name, 
                    'auth_token': client.auth_token
                    }
            res = requests.post(self.url + '/db-disconnect-client', json=data)
            assert(res.status_code == 200)
        except Exception as e :
            raise_error(e, res)
        return res
    
    def list_clients(self):
        try:
            res = requests.get(self.url + '/db-list-clients')
            assert(res.status_code == 200)
        except Exception as e :
            raise_error(e, res)
        return res
    
    def list_categories(self):
        try:
            res = requests.get(self.url + '/db-list-categories')
            assert(res.status_code == 200)
        except Exception as e :
            raise_error(e, res)
        return res
    
    def get_path(self):
        return self.path
    
    def get_url(self):
        return self.url
    
    
    
