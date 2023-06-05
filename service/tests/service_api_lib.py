# Class that is used for database wide operations that don't require authorization
class Buddy:
    def __init__(self, url, path):
        self.path = path
        self.url = url
    
    def get_path(self):
        return self.path
    
    def get_url(self):
        return self.url
    
    def create_client(self, client: Client):
        return
    
    def connect_client(self, client: Client):
        return
    
    def disconnect_client(self, client: Client):
        return
    
    def list_clients(self):
        return
    
    def list_categories(self):
        return
    
    def disconnect(self):
        return
    
    def connect(self):
        return
    
    def create(self):
        return

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
    
    
    
    

def create_buddy(path) :
    data = {
        "path": path,
    }
    res = requests.post('http://localhost:8787/db-create-buddy', json=data)
    assert(res.status_code == 200)
    body = res.json()
    path_new = body['folder_path']
    return path_new