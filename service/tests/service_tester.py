import requests
import json

# HTTP GET request
get_response = requests.get('http://localhost:8787/db-hello')
print('GET Response:', get_response.text)

# JSON object for POST request
data = {
    "key": "bwhat?"
}

# HTTP POST request
post_response = requests.post('http://localhost:8787/db-get', json=data)
print('POST Response:', post_response.text)
