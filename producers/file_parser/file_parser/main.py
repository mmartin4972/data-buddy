import sys
import os
script_path = os.path.abspath(__file__)
parent_directory = os.path.dirname(script_path)
# sys.path.append(os.path.join(parent_directory, '../../service/api-libs'))
from service_api_lib import Buddy, Client, check_success
import argparse
from finance.mint_transactions import MintTransactionParser
import pandas as pd
import pdb

# REQUIRES
# parser class provides is_type method which returns true or false depending on if file is of type
# parser class provides parse method which returns list of json objects parsed according to category
parser_classes = [
    MintTransactionParser()
]

# Use argparse to parse arguments
parser = argparse.ArgumentParser(description='Parse a file or folder of files and write to database',
                                epilog='''This application is unable to create a new data buddy database.\n 
                                        It is unable to create a new category\n
                                        If a folder is specified all files in the folder should be of the same type\n''')
parser.add_argument(dest='path', type=str, nargs=1,
                    help='file to parse or folder with files to parse')
parser.add_argument(dest='category', type=str, nargs=1,
                    help='category of file to parse')
parser.add_argument('-t', '--type', dest='parser_type', type=str, nargs='?',
                    help='type of parser to use to parse file')
parser.add_argument('-u', '--url', dest='db_url', type=str, nargs='?',
                    help='url of data buddy service', default = 'http://localhost:8787')
parser.add_argument('-ml', '--machine-learning', dest='use_ml', action='store_true',)
args = parser.parse_args()
args.path = args.path[0]
args.category = args.category[0]

# Check if path is a file or directory and check if exists
if os.path.isfile(args.path) :
    file_paths = [args.path]
elif os.path.isdir(args.path) :
    file_paths = [os.path.join(args.path, f) for f in os.listdir(args.path) if os.path.isfile(os.path.join(args.path, f))]
else :
    print(f'Path: {args.path} is not a file or directory')
    sys.exit(1)

# Check if can connect to database
b = None
c = None
password = 'file_parser_super_secret_password'
try :
    b = Buddy(args.db_url)
    check_success(b.ping())
    check_success(b.is_connected())
    res = b.list_clients()
    c = Client('file_parser', password)
    if c.name in res.json()['clients'] :
        check_success(b.connect_client(c)) # error here connecting!!! Maybe already connected?
    else :
        check_success(b.create_client(c))
except Exception as e :
    print(e)
    print('Unable to connect to database')
    sys.exit(1)

# Check if category is in list of categories
res = check_success(b.list_categories())
categories = res.json()['categories']
if args.category not in categories :
    print('Category not in list of categories')
    sys.exit(1)

# Determine file type
parser_class = None
if args.parser_type is not None :
    # Check if type is in list of parser classes
    for p in parser_classes :
        if p.name == args.parser_type :
            if not p.is_type(file_paths[0], args.category) :
                print('Type does not match file')
                sys.exit(1)
            parser_class = p
            break
    if parser_class is None :
        print('Type not in list of parser classes')
        sys.exit(1)
else :
    print('type was not specified')
    # Go through parsers and see if we can figure it out
    try :
        for p in parser_classes :
            if p.is_type(file_paths[0], args.category) and (parser_class is not None) :
                raise Exception('Multiple parsers match file')
            if p.is_type(file_paths[0], args.category) :
                parser_class = p
        if parser_class is None :
            raise Exception('No parser matches file')
    except Exception as e :
        print(e)
        print('Unable to determine file type')
        sys.exit(1)
print('determined type')
# Parse file(s)
for f in file_paths :
    vals = parser_class.parse(f)
    for row in vals :
        check_success(c.put(row['category'], row['key'], row['value']))
    print('Parsed file: ' + f)
