import requests
import json
import time
import subprocess
import os
import signal

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

try:
# Run the service
    command = "/data-buddy/service/build/data-buddy-exe"
    process = subprocess.Popen(command, shell=True)
    time.sleep(0.5)

# Check Test Endpoint 1
    get_response = requests.get('http://localhost:8787/db-hello')
    assert(get_response.text == '{"message":"Hello World!"}')

# Check Test Endpoint 2
    data = {
        "key": "bwhat?"
    }
    post_response = requests.post('http://localhost:8787/db-get-test', json=data)
    assert(post_response.text == 'a{"key":"bwhat?"}')

# Check Get Endpoing
    data = {
        "auth_token": "test_token",
        "group": "test_group",
        "category": "test_category",
        "key_params": {
            "a": "b",
            "c": "d",
            "e": "f"
        }
    }
    post_response = requests.post('http://localhost:8787/db-get', json=data)
    print(post_response.json())
    
    print ("All tests passed!")
    
except AssertionError as error:
    kill_process_and_children(process.pid)
    print("Failed")

except:
    kill_process_and_children(process.pid)
    print("Something really went wrong")