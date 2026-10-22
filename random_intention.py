import hashlib
import requests
import json
import sys

def get_sha3_512_hash(intention):
    # Generate the SHA-3-512 hash of the given intention
    sha3_512 = hashlib.sha3_512(intention.encode()).hexdigest()
    # Convert the hash to an integer
    hash_integer = int(sha3_512, 16)
    return hash_integer

def get_random_numbers(api_key, n=8, min_value=1, max_value=1000000000):
    # Generate n random numbers between min_value and max_value using random.org API
    payload = {
        "jsonrpc": "2.0",
        "method": "generateIntegers",
        "params": {
            "apiKey": api_key,
            "n": n,
            "min": min_value,
            "max": max_value,
            "replacement": True
        },
        "id": 1
    }
    
    try:
        response = requests.post("https://api.random.org/json-rpc/4/invoke", json=payload)
        data = response.json()
        
        if 'result' in data:
            random_numbers = data['result']['random']['data']
            return random_numbers
        else:
            print(f"Error: {data.get('error', {}).get('message', 'Unknown error fetching random numbers')}")
            return None
    except Exception as e:
        print(f"An error occurred while fetching random numbers: {e}")
        return None

def calculate_new_value(hash_integer, random_numbers):
    # Multiply the random numbers together
    number_multiplied = 1
    for number in random_numbers:
        number_multiplied *= number

    # Calculate hash_integer % number_multiplied
    new_value = hash_integer % number_multiplied
    return new_value

def load_api_key(filename):
    try:
        with open(filename, 'r') as file:
            api_key = file.read().strip()
            return api_key
    except FileNotFoundError:
        print(f"Please put the API Key in {filename}.")
        sys.exit(1)

def main():
    # Step 1: Take an input "Intention: "
    intention = input("Intention: ")

    # Step 2: Get the SHA-3-512 hash and convert to an integer
    hash_integer = get_sha3_512_hash(intention)

    # Step 3: API key for random.org (ensure this is set correctly)
    random_org_api_key = load_api_key("RANDOMORG_API_KEY.txt")

    # Step 4: Generate 8 random numbers using the random.org API
    random_numbers = get_random_numbers(random_org_api_key)
    
    if not random_numbers:
        print("Failed to retrieve random numbers.")
        return
    
    # Step 5 & 6: Multiply the random numbers and calculate the new value
    new_value = calculate_new_value(hash_integer, random_numbers)

    # Step 7: Print the final result
    print(f"True Random Influenced Intention Number: {new_value}")

if __name__ == "__main__":
    main()
