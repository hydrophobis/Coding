import hashlib
import time

# Function to generate SHA256 hash of a string
def sha256(s):
    return hashlib.sha256(s.encode()).hexdigest()

# Function to generate combinations using recursive backtracking
def generate_combinations(char_set, max_length, password, current_string, guesses):
    if sha256(current_string) == password:
        elapsed_time = time.time() - generate_combinations.start_time
        print(f"\n\n\n\nFound {password} ({current_string}) with {guesses} guesses in {elapsed_time:.6f} seconds")
        exit(0)

    for c in char_set:
        current_string += c
        guesses += 1

        if len(current_string) <= max_length:
            generate_combinations(char_set, max_length, password, current_string, guesses)

        current_string = current_string[:-1]

def main():
    char_set = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()-_=+[]{}|,'.<>?~"

    password = input("Hashed Input Password: ")
    max_length = int(input("Length Before Hashing: "))

    current_string = ""
    guesses = 0

    generate_combinations.start_time = time.time()
    generate_combinations(char_set, max_length, password, current_string, guesses)

if __name__ == "__main__":
    main()
