import json
import pandas as pd

class Singleton:
    _instance = None

    def __new__(cls):
        if cls._instance is None:
            cls._instance = super().__new__(cls)
        return cls._instance

class PredefinedObjectsJSON(Singleton) :
    def __init__(self) :
        with open('../../../predefined_objects.json', 'r') as f :
            self.predefined_json = json.load(f)

    def __getitem__(self, index):
        return self.predefined_json[index]
    
def get_finance_types() :
    return PredefinedObjectsJSON()['user_schemas']['finance']['value_schema']['properties']['transaction_type']['enum']

def get_finance_subtypes() :
    types = []
    for obj in PredefinedObjectsJSON()['user_schemas']['finance']['value_schema']['oneOf'] :
        types.append(obj['then']['properties']['transaction_subtype']['enum'])
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
