import hashlib
import requests
import json
import sys
import argparse

def get_sha3_512_hash(intention):
    # Generate the SHA-3-512 hash of the given intention
    sha3_512 = hashlib.sha3_512(intention.encode()).hexdigest()
    # Convert the hash to an integer
    hash_integer = int(sha3_512, 16)
    return hash_integer

def get_random_numbers(n=8, min_value=1, max_value=1000000000):
    # Uses the direct RANDOM.ORG HTTP GET interface (IP-quota based)
    url = "https://www.random.org/integers/"
    params = {
        "num": n,
        "min": min_value,
        "max": max_value,
        "col": 1,
        "base": 10,
        "format": "plain",
        "rnd": "new"
    }
    headers = {
        # RANDOM.ORG guidelines recommend specifying an identifier in User-Agent
        "User-Agent": "IntentionApp/1.0"
    }

    try:
        response = requests.get(url, params=params, headers=headers)
        if response.status_code == 200:
            # Parse plain-text response into a list of integers
            random_numbers = [int(line.strip()) for line in response.text.strip().splitlines() if line.strip()]
            return random_numbers
        else:
            print(f"Error: HTTP {response.status_code} - {response.text.strip()}")
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
    # Parse command-line arguments
    parser = argparse.ArgumentParser(description="True Random Influenced Intention Number Generator")
    parser.add_argument("-i", "--intention", type=str, help="The intention string to process")
    args = parser.parse_args()

    # Step 1: Use the -i argument if provided, otherwise prompt the user
    if args.intention is not None:
        intention = args.intention
    else:
        intention = input("Intention: ")

    # Step 2: Get the SHA-3-512 hash and convert to an integer
    hash_integer = get_sha3_512_hash(intention)

    # Step 3 & 4: Generate 8 random numbers using the HTTP API (no API key needed)
    random_numbers = get_random_numbers()

    if not random_numbers:
        print("Failed to retrieve random numbers.")
        return

    # Step 5 & 6: Multiply the random numbers and calculate the new value
    new_value = calculate_new_value(hash_integer, random_numbers)

    # Step 7: Print the final result
    print(f"True Random Influenced Intention Number: {new_value}")

if __name__ == "__main__":
    main()
