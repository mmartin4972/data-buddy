import csv
import datetime
import sys
import json
import pandas as pd
from finance_common import *

class MintTransactionParser : 
    def __init__(self, name, use_ml) :
        self.name = name
        self.category = 'finance'

        if use_ml :
            import spacy
            self.nlp = spacy.load('en_core_web_sm')
        else :
            self.nlp = None
    
    def format_mint_category(self, mint_category) :
        mint_category = mint_category.lower()
        mint_category = mint_category.replace(' ', '_')
        mint_category = mint_category.replace('&', 'and')
        return mint_category

    def convert_mint_category_to_finance_type(self, mint_category) :
        final_category = ""
        mint_category = self.format_mint_category(mint_category)
        
        if mint_category in get_finance_types() :
            final_category = mint_category
        elif mint_category in get_finance_subtypes() :
            final_category = get_finance_subtypes_to_type()[mint_category]
        elif self.nlp is not None : # Use ML to determine category
            print("Add ML code here")
        
        return final_category
            
    def convert_mint_category_to_finance_subtype(self, mint_category) :
        final_category = ""
        mint_category = self.format_mint_category(mint_category)

        if mint_category in get_finance_subtypes() :
            final_category = ""
        elif self.nlp is not None :
            print("Add ML code for subtypes here")
        
        return final_category

    def is_type(self, file_path, category) :
        if category == self.category :
            return False

        try :
            with open(file_path, 'r') as f :
                reader = csv.reader(f)
                row = reader.__next__()
                # First Row
                if row != ['Date', 'Description', 'Original Description', 'Amount', 'Transaction Type', 'Category', 'Account Name', 'Labels', 'Notes'] :
                    raise Exception()
                # Second Row if it exists
                row = reader.__next__()
                if row is not None :
                    datetime.strptime(row[0], '%m/%d/%Y')
                    if row[1].isdigit() :
                        raise Exception()
                    if row[2].isdigit() :
                        raise Exception()
                    float(row[3])
                    if row[4] != 'debit' and row[4] != 'credit' :
                        raise Exception()
                    if row[5].isalpha() == False :
                        raise Exception()
        except Exception as e :
            return False
        return True
    
    def parse(self, file_path) :
        def sort_by_date(row) :
            return datetime.strptime(row[0], '%m/%d/%Y')
        
        raw_data = pd.read_csv(file_path)
        raw_data.sort_values(by=0, key=sort_by_date, inplace=True)
        vals = []
        # Transaction time granularity is to the day. ISO 8601 format includes hours, minutes, and seconds.
        # To avoid duplicate transactions we increment the time by 1 second for each transaction if it is the same day as the prior transaction
        most_recent_datetime_parsed = datetime.utcfromtimestamp(0) # Unix epoch start date
            
        for _, row in raw_data.iterrows() :
            # Make sure all time values are unique
            time = datetime.strptime(row[0], '%m/%d/%Y')    
            if time.date() == most_recent_datetime_parsed.date() :
                time = most_recent_datetime_parsed + datetime.timedelta(seconds=1)
            most_recent_datetime_parsed = time

            type = self.convert_mint_category_to_finance_type(row[5])
            subtype = self.convert_mint_category_to_finance_subtype(row[5])

            vals.append({
                "category" : "finance",
                "key" : {
                    "category" : "finance", 
                    "time" : time.isoformat()
                    },
                "value" : {
                    "name" : row[2],
                    "time" : time.isoformat(),
                    "amount" : row[3],
                    "direction" : row[4],
                    "account" : row[6],
                    "source" : "intuit mint",
                    "description" : row[1],
                    "transaction_type" : type,
                    "transaction_subtype" : subtype
                }
            })

        return vals
