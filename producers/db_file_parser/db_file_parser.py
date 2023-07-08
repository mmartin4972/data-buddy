import sys
import argparse
from finance.mint_transactions import mint_transaction_parser

# Use argparse to parse arguments
parser = argparse.ArgumentParser(description='Parse a file or folder of files and write to database')
parser.add_argument(dest='file', type=str, nargs=1,
                    help='file to parse or folder with files to parse')
parser.add_argument(dest='category', type=str, nargs=1,
                    help='category of file to parse')
parser.add_argument('-t', '--type', dest='parser_type', type=str, nargs='?',
                    help='type of parser to use to parse file')
parser.add_argument('-d', '--database', dest='db_path', type=str, nargs='?',
                    help='path of data buddy folder')
parser.add_argument('-u', '--url', dest='db_url', type=str, nargs='?',
                    help='url of data buddy service')
args = parser.parse_args()

# REQUIRES
# parser class provides is_type method which returns true or false depending on if file is of type
# parser class provides parse method which returns list of json objects parsed according to category
parser_classes = [
    mint_transaction_parser
]
# In main do write to database
# Remember to get a list of all categories and make sure that provided category is in list
