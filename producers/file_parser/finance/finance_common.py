import pandas as pd
import sys
import os
script_path = os.path.abspath(__file__)
parent_directory = os.path.dirname(script_path)
sys.path.append(os.path.join(parent_directory, '../'))
from common import IFileParser, PredefinedObjectsJSON

def format_mint_category(mint_category) :
    mint_category = mint_category.lower()
    mint_category = mint_category.replace(' ', '_')
    mint_category = mint_category.replace('&', 'and')
    return mint_category

def get_finance_types() :
    return PredefinedObjectsJSON()['user_schemas']['finance']['value_schema']['properties']['transaction_type']['enum']

def get_finance_subtypes() :
    types = []
    for obj in PredefinedObjectsJSON()['user_schemas']['finance']['value_schema']['oneOf'] :
        types += (obj['then']['properties']['transaction_subtype']['enum'])
    return types

def get_finance_subtypes_to_type() :
    types = {}
    for obj in PredefinedObjectsJSON()['user_schemas']['finance']['value_schema']['oneOf'] :
        for subtype in obj['then']['properties']['transaction_subtype']['enum'] :
            types[subtype] = obj['if']['properties']['transaction_type']['const']
    return types
    
def get_finance_df() : 
    cols = list(PredefinedObjectsJSON()['user_schemas']['finance']['value_schema']['properties'].keys())
    return pd.DataFrame(columns=cols)
