import requests
import json
import time
import subprocess
import os
import signal
import traceback
import shutil

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
    body = res.json()
    assert(body['error'] == "")
    assert(body['folder_path'] == (path + '/data_buddy'))
    assert(os.path.isdir(body['folder_path']))
    assert(os.path.isdir(body['folder_path'] + '/app_db'))
    assert(os.path.isdir(body['folder_path'] + '/user_db'))
    
    # Disconnect and clean up
    res = requests.get('http://localhost:8787/db-disconnect-buddy')
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

# Build the service
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
             connect_buddy_endpoint_error_invalid_path
            ]
try:
# Run the service
    command = "/data-buddy/service/build/data-buddy-exe"
    process = subprocess.Popen(command, shell=True)
    time.sleep(0.5)

    path = "/testing"
    if (os.path.isdir(path)):
        shutil.rmtree(path)
    
    for func in func_list:
        os.mkdir(path)
        func(path)
        shutil.rmtree(path)

    # os.mkdir(path)
    # connect_endpoint_error_already_connected(path)
    
    print ("All tests passed!")
    
except AssertionError as error:
    traceback.print_exc()
    print("Failed")

except:
    traceback.print_exc()
    print("Something really went wrong")
    
# shutil.rmtree(path) 
kill_process_and_children(process.pid)
