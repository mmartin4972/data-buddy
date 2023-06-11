import requests
import json
import time
import subprocess
import os
import signal
import traceback
import shutil
import sys
import pdb
from datetime import datetime, timedelta

# Helper Functions
def kill_process_and_children(pid):
    try:
        output = subprocess.check_output(['pgrep', '-P', str(pid)])
        child_pids = [int(pid) for pid in output.decode().split()]
        for child_pid in child_pids:
            kill_process_and_children(child_pid)
    except subprocess.CalledProcessError:
        pass

    os.kill(pid, signal.SIGTERM)

# Check Test Endpoint 1
def test_endpoint_1(path) :
    get_response = requests.get('http://localhost:8787/db-hello')
    assert(get_response.text == '{"message":"Hello World!"}')

# Check Test Endpoint 2
def test_endpoint_2(path):
    data = {
        "key": "bwhat?"
    }
    post_response = requests.post('http://localhost:8787/db-get-test', json=data)
    assert(post_response.text == '{"key":"bwhat?"}')

# Check Create Buddy Endpoint and Disconnect Endpoint Success
def check_create_buddy_and_disconnect_success(path):
    data = {
        "path": path,
    }
    res = requests.post('http://localhost:8787/db-create-buddy', json=data)
    assert (res.status_code == 200)
    print("got response")
    body = res.json()
    assert(body['error'] == "")
    assert(body['folder_path'] == (path + '/data_buddy'))
    assert(os.path.isdir(body['folder_path']))
    assert(os.path.isdir(body['folder_path'] + '/app_db'))
    assert(os.path.isdir(body['folder_path'] + '/user_db'))
    
    # Disconnect and clean up
    res = requests.get('http://localhost:8787/db-disconnect-buddy')
    print("disconnected")
    assert(res.status_code == 200)

# Check Create Buddy Endpoint Error (Invalid Path) and Disconnect close not opened
def create_buddy_endpoint_error_invalid_path(path) :
    path_wrong = "/hmm/what/are/you/doing"
    data = {
        "path": path_wrong,
    }
    res = requests.post('http://localhost:8787/db-create-buddy', json=data)
    assert (res.status_code == 500)
    assert(res.json()['error'] != "")

# Check Connect Buddy Endpoint Success
def connect_endpoint_success(path) :
    data = {
        "path": path,
    }
    res = requests.post('http://localhost:8787/db-create-buddy', json=data)
    assert(res.status_code == 200)
    body = res.json()
    path_new = body['folder_path']
    
    res = requests.get('http://localhost:8787/db-disconnect-buddy')
    assert(res.status_code == 200)
    
    data = {
        "path": path_new
    }
    res2 = requests.post('http://localhost:8787/db-connect-buddy', json=data)
    assert(res2.status_code == 200)
    assert(res2.json()['error'] == "")
    
    # Disconnect and Clean Up
    res = requests.get('http://localhost:8787/db-disconnect-buddy')
    assert(res.status_code == 200)
    
# Check Connect Buddy Endpoint Error (Already Connected)
def connect_endpoint_error_already_connected(path):
    data = {
        "path": path,
    }
    res = requests.post('http://localhost:8787/db-create-buddy', json=data)
    assert(res.status_code == 200)
    body = res.json()
    path_new = body['folder_path']
    
    res = requests.get('http://localhost:8787/db-disconnect-buddy')
    assert(res.status_code == 200)
    
    data = {
        "path": path_new
    }
    res = requests.post('http://localhost:8787/db-connect-buddy', json=data)
    assert(res.status_code == 200)
    assert(res.json()['error'] == "")
    
    res = requests.post('http://localhost:8787/db-connect-buddy', json=data)
    assert(res.status_code == 500)
    assert(res.json()['error'] != "")
    
    # Disconnect and Clean Up
    res = requests.get('http://localhost:8787/db-disconnect-buddy')
    assert(res.status_code == 500)

# Check Connect Buddy Endpoint Error (Invalid Path)
def connect_buddy_endpoint_error_invalid_path(path) :
    path_wrong = "/hmm/what/are/you/doing"
    data = {
        "path": path_wrong,
    }
    res = requests.post('http://localhost:8787/db-connect-buddy', json=data)
    assert (res.status_code == 500)
    assert(res.json()['error'] != "")
    
# Check Get Endpoint
    # data = {
    #     "auth_token": "test_token",
    #     "group": "test_group",
    #     "category": "test_category",
    #     "key_params": json.dumps({
    #         "a": "b",
    #         "c": "d",
    #         "e": "f"
    #     })
    # }
    # post_response = requests.post('http://localhost:8787/db-get', json=data)
    # print(post_response.text)


from service_api_lib import * 

def check_success(res) :
    assert(res.status_code == 200)
    if res.json()['error'] != "":
        print(res.json()['error'])
    assert(res.json()['error'] == "")

def check_fail(res) :
    if res.json()['error'] != "":
        print(res.json()['error'])
    assert(res.status_code != 200)
    
def check_api_success(path):
    b = Buddy("http://localhost:8787")
    check_success(b.create(path))
    check_fail(b.connect())
    check_fail(b.disconnect()) # since the prior command should have disconnected you
    check_success(b.connect())
    check_success(b.disconnect())

def check_create_client_success(path):
    b = Buddy("http://localhost:8787")
    c = Client("test client", "test_password")
    c1 = Client("test client1", "test_password1")
    # pdb.set_trace()
    check_success(b.create(path))
    res = b.create_client(c)
    check_success(res)
    
    res = b.create_client(c1)
    check_success(res)

    check_success(b.disconnect())

def check_create_same_client_fail(path):
    b = Buddy("http://localhost:8787")
    c = Client("test client", "test password")
    check_success(b.create(path))
    res = b.create_client(c)
    check_success(res)
    
    res = b.create_client(c)
    check_fail(res)

    check_success(b.disconnect())
    
def check_connect_disconnect_client_success(path):
    b = Buddy("http://localhost:8787")
    c = Client("test client", "test password")
    check_success(b.create(path))
    check_success(b.create_client(c))
    check_fail(b.create_client(c))
    check_success(b.disconnect_client(c))
    check_fail(b.create_client(c))
    check_success(b.connect_client(c))
    check_success(b.disconnect_client(c))
    check_success(b.disconnect())

def check_double_disconnect_client(path):
    b = Buddy("http://localhost:8787")
    c = Client("test client", "test password")
    check_success(b.create(path))
    check_success(b.create_client(c))
    check_success(b.disconnect_client(c))
    check_fail(b.disconnect_client(c))
    check_success(b.disconnect())
    
def check_list_clients(path):
    b = Buddy("http://localhost:8787")
    c = Client("test client", "test password")
    c1 = Client("test_client1", "test_password2")
    check_success(b.create(path))
    res = b.list_clients()
    check_success(res)
    assert(json.loads(res.json()['clients']) == json.loads("[]"))
    check_success(b.create_client(c))
    check_success(b.create_client(c1))
    res = b.list_clients()
    check_success(res)
    val = res.json()['clients']
    check = '''["test client","test_client1"]'''
    assert(json.loads(val) == json.loads(check))  
    check_success(b.disconnect())

transaction_key_schema = {
    'type': 'object',
    'properties': {
        'category': { 'type': 'string' },
        'time': { 
            'type': 'string',
            'format': 'date-time'
        }
    },
    'required': ['category','time']
}

transaction_value_schema = {
    'type': 'object',
    'properties': {
        'name': { 'type': 'string' },
        'amount': { 'type': 'number' },
        'time': { 
            'type': 'string',
            'format': 'date-time'
            },
        'place': { 'type': 'string' }
    },
    'required': ['name','amount','time','place']
}

def build_transaction_key(category, time):
    return {
        "category": category,
        "time": time
    }

def build_transaction_value(name, amount, time, place):
    return {
        "name": name,
        "amount": amount,
        "time": time,
        "place": place
    }

def create_category_success(path):
    b = Buddy("http://localhost:8787")
    c = Client("test client", "test_password")
    check_success(b.create(path))
    check_success(b.create_client(c))
    check_success(c.create_category("test_category", transaction_key_schema, transaction_value_schema))
    check_fail(c.create_category("test_category", transaction_key_schema, transaction_value_schema))
    check_success(b.disconnect())
    
def list_categories_success(path):
    b = Buddy("http://localhost:8787")
    c = Client("test client", "test_password")
    check_success(b.create(path))
    check_success(b.create_client(c))
    check_success(c.create_category("test_category", transaction_key_schema, transaction_value_schema))
    check_fail(c.create_category("test_category", transaction_key_schema, transaction_value_schema))
    check_success(c.create_category("test_category2", transaction_key_schema, transaction_value_schema))
    res = b.list_categories()
    check_success(res)
    
    data = [{'clients': ['test client'], 'key_schema': {'properties': {'category': {'type': 'string'}, 'time': {'format': 'date-time', 'type': 'string'}}, 'required': ['category', 'time'], 'type': 'object'}, 'name': 'test_category2', 'value_schema': {'properties': {'amount': {'type': 'number'}, 'name': {'type': 'string'}, 'place': {'type': 'string'}, 'time': {'format': 'date-time', 'type': 'string'}}, 'required': ['name', 'amount', 'time', 'place'], 'type': 'object'}}, {'clients': ['test client'], 'key_schema': {'properties': {'category': {'type': 'string'}, 'time': {'format': 'date-time', 'type': 'string'}}, 'required': ['category', 'time'], 'type': 'object'}, 'name': 'test_category', 'value_schema': {'properties': {'amount': {'type': 'number'}, 'name': {'type': 'string'}, 'place': {'type': 'string'}, 'time': {'format': 'date-time', 'type': 'string'}}, 'required': ['name', 'amount', 'time', 'place'], 'type': 'object'}}]
    parsed = json.loads(res.json()['categories'])
    assert(parsed == data)
    check_success(b.disconnect())
    
def unauthorized_calls(path):
    b = Buddy("http://localhost:8787")
    c = Client("test client", "test_password")
    c1 = Client("test client1", "test_password1")
    check_success(b.create(path))
    check_fail(b.disconnect_client(c))
    check_success(b.create_client(c))
    check_success(b.create_client(c1))
    check_success(b.disconnect_client(c))
    
    # Unauthorized calls
    check_fail(c.create_category("test_category", transaction_key_schema, transaction_value_schema))
    check_fail(c.add_client_to_category("test_category", c))
    
    # Authorized calls
    res = b.connect_client(c)
    check_success(res)
    check_success(c.create_category("test_category", transaction_key_schema, transaction_value_schema))
    check_fail(c.add_client_to_category("test_category", c))
    check_fail(c1.add_client_to_category("test_category", c1))
    check_success(b.disconnect())
    
def add_client_to_category_success(path) :
    b = Buddy("http://localhost:8787")
    c = Client("test client", "test_password")
    c1 = Client("test client1", "test_password1")
    check_success(b.create(path))
    check_success(b.create_client(c))
    # Should fail since test client1 doesn't exist
    check_success(c.create_category("test_category", transaction_key_schema, transaction_value_schema))
    check_fail(c.add_client_to_category("test_category", c1))
    check_success(b.create_client(c1))
    check_fail(c1.create_category("test_category", transaction_key_schema, transaction_value_schema))
    check_success(c.add_client_to_category("test_category", c1))
    check_fail(c1.create_category("test_category", transaction_key_schema, transaction_value_schema))
    # Add disconnected client to category
    check_success(b.disconnect_client(c1))
    check_success(c.create_category("test_category2", transaction_key_schema, transaction_value_schema))
    check_success(c.add_client_to_category("test_category2", c1))
    check_fail(c.add_client_to_category("test_category2", c))
    check_success(b.disconnect_client(c))
    res = b.list_categories()
    check_success(res)
    ans = {'error': '', 'categories': '[{"clients":["test client","test client1"],"key_schema":{"properties":{"category":{"type":"string"},"time":{"format":"date-time","type":"string"}},"required":["category","time"],"type":"object"},"name":"test_category2","value_schema":{"properties":{"amount":{"type":"number"},"name":{"type":"string"},"place":{"type":"string"},"time":{"format":"date-time","type":"string"}},"required":["name","amount","time","place"],"type":"object"}},{"clients":["test client","test client1"],"key_schema":{"properties":{"category":{"type":"string"},"time":{"format":"date-time","type":"string"}},"required":["category","time"],"type":"object"},"name":"test_category","value_schema":{"properties":{"amount":{"type":"number"},"name":{"type":"string"},"place":{"type":"string"},"time":{"format":"date-time","type":"string"}},"required":["name","amount","time","place"],"type":"object"}}]'}
    assert(b.list_categories().json() == ans)
    check_success(b.disconnect())

def get_put(path) :
    b = Buddy("http://localhost:8787")
    c = Client("test client", "test_password")
    c1 = Client("test client1", "test_password1")
    check_success(b.create(path))
    check_success(b.create_client(c))
    check_success(b.create_client(c1))
    
    # Authorized calls
    check_success(c.create_category("transaction", transaction_key_schema, transaction_value_schema))
    time_string_bad = "45"
    
    current_time = datetime.utcnow()
    time_string = current_time.isoformat("T") + "Z"
    check_success(c.put("transaction", build_transaction_key("transaction", time_string), build_transaction_value("test_name", 1.0, time_string, "test_place")))
    check_success(c.put("transaction", build_transaction_key("transaction", time_string), build_transaction_value("test_name", 1.0, time_string, "test_place"))) # duplicate puts should succeed
    check_fail(c.put("transaction", build_transaction_key("transaction", time_string_bad), build_transaction_value("test_name", 1.0, time_string_bad, "test_place"))) # Should fail format check
    res = c.get("transaction", build_transaction_key("transaction", time_string))
    check_success(res)
    assert(json.loads(res.json()['value']) == build_transaction_value("test_name", 1.0, time_string, "test_place"))
    
    # Unauthorized calls
    current_time = datetime.utcnow()
    time_string = current_time.isoformat("T") + "Z"
    check_fail(c1.put("transaction", build_transaction_key("transaction", time_string), build_transaction_value("test_name", 1.0, time_string, "test_place")))
    check_fail(c1.get("transaction", build_transaction_key("transaction", time_string)))
    
    # Unauthorized that should now succeed
    check_success(c.add_client_to_category("transaction", c1))
    check_success(c1.put("transaction", build_transaction_key("transaction", time_string), build_transaction_value("test_name", 1.0, time_string, "test_place")))
    res = c1.get("transaction", build_transaction_key("transaction", time_string))
    check_success(res)
    assert(json.loads(res.json()['value']) == build_transaction_value("test_name", 1.0, time_string, "test_place"))
    check_success(b.disconnect())

def get_range_put(path) :
    b = Buddy("http://localhost:8787")
    c = Client("test client", "test_password")
    c1 = Client("test client1", "test_password1")
    check_success(b.create(path))
    check_success(b.create_client(c))
    check_success(b.create_client(c1))
    check_success(c.create_category("transaction", transaction_key_schema, transaction_value_schema))
    
    current_time = datetime.utcnow() - timedelta(hours=1)
    time_string = current_time.isoformat("T") + "Z"
    check_success(c.put("transaction", build_transaction_key("transaction", time_string), build_transaction_value("test_name", 1.0, time_string, "test_place")))
    
    current_time = datetime.utcnow()
    time_string1 = current_time.isoformat("T") + "Z"
    check_success(c.put("transaction", build_transaction_key("transaction", time_string1), build_transaction_value("test_name", 2.0, time_string1, "test_place")))

    current_time = datetime.utcnow()
    time_string2 = current_time.isoformat("T") + "Z"
    check_success(c.put("transaction", build_transaction_key("transaction", time_string2), build_transaction_value("test_name", 3.0, time_string2, "test_place")))
    
    res = c.get_range("transaction", build_transaction_key("transaction", ""))
    check_success(res)
    assert(json.loads(res.json()['values']) == [build_transaction_value("test_name", 1.0, time_string, "test_place"), build_transaction_value("test_name", 2.0, time_string1, "test_place"), build_transaction_value("test_name", 3.0, time_string2, "test_place")])
    
    res = c.get_range("transaction", build_transaction_key("transaction", time_string1[0:14]))
    check_success(res)
    assert(json.loads(res.json()['values']) == [build_transaction_value("test_name", 2.0, time_string1, "test_place"), build_transaction_value("test_name", 3.0, time_string2, "test_place")])

    check_success(b.disconnect())
    
    
test = ""
if len(sys.argv) > 1:
    test = sys.argv[1]   

# Build the service
process = None
if test == "": # don't recompile if a specific test is being checked
    command = "cd /data-buddy/service/build; make"
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output, error = process.communicate()
    error_code = process.returncode

    if error_code == 0:
        print("Build Successful")
    else:
        print("Build Failed")
        print("Error code:", error_code)
        print("Output:")
        print(output.decode())
        print("Error:")
        print(error.decode())
        exit()

    process = None

func_list = [test_endpoint_1, 
            test_endpoint_2, 
            check_create_buddy_and_disconnect_success, 
            create_buddy_endpoint_error_invalid_path, 
            connect_endpoint_success, 
            connect_endpoint_error_already_connected,
            connect_buddy_endpoint_error_invalid_path,
            check_api_success,
            check_create_client_success,
            check_create_same_client_fail,
            check_connect_disconnect_client_success,
            check_double_disconnect_client,
            check_list_clients,
            create_category_success,
            list_categories_success,
            unauthorized_calls,
            add_client_to_category_success,
            get_put,
            get_range_put
            ]
try:
# Run the service
    if test == "": # Don't run the service if a specific test is being checked
        command = "/data-buddy/service/build/data-buddy-exe"
        process = subprocess.Popen(command, shell=True)
        time.sleep(0.5)

# Run the tests
    path = "/testing"
    if (os.path.isdir(path)):
        shutil.rmtree(path)
    for func in func_list:
        if (test == "" or func.__name__ == test):
            print("[TEST]", func.__name__)
            os.mkdir(path)
            func(path)
            shutil.rmtree(path)
            print("[PASSED]")
    
    print ("All tests passed!")
    
except AssertionError as error:
    traceback.print_exc()
    print("Failed")

except:
    traceback.print_exc()
    print("Something really went wrong")
    
# shutil.rmtree(path)
if process != None:
    kill_process_and_children(process.pid)
