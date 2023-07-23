import pandas as pd
import sys
import os
script_path = os.path.abspath(__file__)
parent_directory = os.path.dirname(script_path)
# sys.path.append(os.path.join(parent_directory, '../'))
from ..common import IFileParser, PredefinedObjectsJSON, datetime_to_rfc_3339

def format_mint_category(mint_category) :
    mint_category = mint_category.lower()
    mint_category = mint_category.replace(' ', '_')
    mint_category = mint_category.replace('&', 'and')
    return mint_category

def get_finance_types() :
    return PredefinedObjectsJSON()['user_schemas']['finance']['value_schema']['properties']['transaction_type']['enum']

def get_finance_subtypes(transaction_type=None) :
    types = set()
    for obj in PredefinedObjectsJSON()['user_schemas']['finance']['value_schema']['oneOf'] :
        if transaction_type is None or transaction_type == obj['properties']['transaction_type']['enum'][0] :
            types.update(obj['properties']['transaction_subtype']['enum'])
    types = list(types)
    return types

def get_finance_subtypes_to_type() :
    types = {}
    for obj in PredefinedObjectsJSON()['user_schemas']['finance']['value_schema']['oneOf'] :
        for subtype in obj['properties']['transaction_subtype']['enum'] :
            types[subtype] = obj['properties']['transaction_type']['enum'][0]
    return types
    
def get_finance_df() : 
    cols = list(PredefinedObjectsJSON()['user_schemas']['finance']['value_schema']['properties'].keys())
    return pd.DataFrame(columns=cols)
