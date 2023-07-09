import csv
import sys
import argparse
import random
import os
from datetime import datetime
script_path = os.path.abspath(__file__)
parent_directory = os.path.dirname(script_path)
sys.path.append(os.path.join(parent_directory, '../finance'))
from finance_common import *

parser = argparse.ArgumentParser(description='Generates fake data for testing mint application')

lines = [["Date", "Description", "Original Description", "Amount", "Transaction Type", "Category", "Account Name", "Labels", "Notes"]]

# Build set of all possible categories
known_types = [
    "Category", "Ride Share", "Transfer", "Shopping", "Restaurants", "Air Travel", "Travel", "Credit Card Payment", "Groceries", "Investments", "Dividend & Cap Gains", "Income", "Hotel", "Utilities", "Mortgage & Rent", "Parking", "Charity", "Shipping", "Entertainment", "Vacation", "Clothing", "Home Supplies", "Movies & DVDs", "Gas & Fuel", "Sporting Goods", "Fees & Charges", "Legal", "Fast Food", "Uncategorized", "Paycheck", "Federal Tax", "State Tax", "Taxes", "Internet", "Education", "Buy", "Bank Fee", "ATM Fee", "Public Transportation", "Books", "Financial", "Sports", "Electronics & Software", "Home Improvement", "Food & Dining", "Service Fee", "Coffee Shops", "Check", "Gym", "Deposit", "Hobbies", "Amusement", "Auto & Transport", "Food Delivery", "Bills & Utilities", "Alcohol & Bars", "Pharmacy", "Doctor", "Sell", "Trade Commissions", "Misc Expenses", "Service & Parts", "Finance Charge", "Interest Income", "Laundry", "Gift", "Cash & ATM", "Tuition", "Books & Supplies", "Gifts & Donations", "Lawn & Garden", "Music", "Personal Care", "Transfer for Cash Spending"
]
for type in known_types :
    type = format_mint_category(type)
types = set(known_types)
types.update(get_finance_types())
types.update(get_finance_subtypes())
types = list(types)

# Set of accounts
accounts = ["Checking", "Savings", "Credit Card", "Investment", "Cash"]
descriptions = ["a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r"]
directions = ["debit", "credit"]

for i in range(1000) : # Generate 1000 transactions
    date = datetime(2023, random.randint(1, 12), random.randint(1, 28)).strftime('%m/%d/%Y')
    description = random.choice(descriptions)
    original_description = description
    amount = random.randint(1, 10000) / 100
    direction = random.choice(directions)
    category = random.choice(types)
    account = random.choice(accounts)
    labels = ""
    notes = ""
    lines.append([date, description, original_description, amount, direction, category, account, labels, notes])

with open('fake_intuit_mint_data.csv', 'w') as f :
    writer = csv.writer(f)
    writer.writerows(lines)
