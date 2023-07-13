import argparse
import subprocess
import time
import requests
import json
import os
import signal
import traceback
import shutil
import sys
import pdb
from datetime import datetime, timedelta
import csv
script_path = os.path.abspath(__file__)
parent_directory = os.path.dirname(script_path)
sys.path.append(os.path.join(parent_directory, '../../../service/api-libs'))
sys.path.append(os.path.join(parent_directory, '../'))
from service_api_lib import Buddy, Client, check_success, check_fail
from finance.finance_common import *
from common import PredefinedObjectsJSON

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

# Create a list of all files to parse
# Check the state of the database after the file is parsed to ensure parsing was successful
# delete the database and start from scratch in the next test
# similar to the other testing application

files_to_parse = [
    ['fake_intuit_mint_data.csv', 'finance']
]

parser = argparse.ArgumentParser(description='Parse a file or folder of files and write to database')
parser.add_argument(dest='file_name', type=str, nargs='?')
parser.add_argument(dest='category', type=str, nargs='?')
args = parser.parse_args()
if (args.file_name is not None and args.category is None) or (args.file_name is None and args.category is not None):
    print('Must provide both file name and category or neither')

process = None
try:
# Run the service
    if args.file_name == None : # Don't run the service if a specific test is being checked
        command = '/data-buddy/service/build/data-buddy-exe'
        process = subprocess.Popen(command, shell=True)
        time.sleep(0.5)
    else :
        files_to_parse = [[args.file_name, args.category]]

# Run the tests
    path = '/testing'
    if (os.path.isdir(path)):
        shutil.rmtree(path)
    
    for obj in files_to_parse:
        file = obj[0]
        category = obj[1]
        print('[TEST]', file)
        rows = []
        # Read in the csv file
        with open(file, newline='') as csvfile:
            reader = csv.reader(csvfile, delimiter=',')
            for row in reader:
                rows.append(row)

        # Setup data buddy
        os.mkdir(path)
        b = Buddy('http://localhost:8787')
        check_success(b.create(path))
        c = Client('test client', 'test_password')
        check_success(b.create_client(c))
        check_success(c.create_category(
            category,
            PredefinedObjectsJSON()['user_schemas'][category]['key_schema'],
            PredefinedObjectsJSON()['user_schemas'][category]['value_schema']
        ))

        # Fail to write data since file_parser doesn't have permission
        run_file_parser_cmd = f'python3 {parent_directory}/../file_parser.py {parent_directory}/../tests/{file} {category}'
        status = os.system(run_file_parser_cmd)
        if os.WEXITSTATUS(status) == 0:
            raise Exception('File parser succeeded when it should have failed')

        # Grant permission
        check_success(c.add_client_to_category(category, 'file_parser'))

        # Successfully write data
        status = os.system(run_file_parser_cmd)
        if os.WEXITSTATUS(status) != 0:
            raise Exception('File parser failed')
        
        # Check the database to make sure data was written
        obj = PredefinedObjectsJSON()['user_schemas'][category]['key_schema']['properties'] 
        key = {}
        for property in obj.keys() :
            if property == 'category' :
                key[property] = category
            elif obj[property]['type'] == 'string' :
                key[property] = ''
            else :
                raise Exception('Unsupported type')
        res = c.get_range(category, key)
        check_success(res)
        parsed = json.loads(res.json()['values'])
        assert(len(parsed) == len(rows)-1)
        print("We have indeed asserted that the length is grand")
        
except AssertionError as error:
    traceback.print_exc()
    print('Failed')

except:
    traceback.print_exc()
    print('Something really went wrong')
    
# shutil.rmtree(path)
if process != None:
    kill_process_and_children(process.pid)
