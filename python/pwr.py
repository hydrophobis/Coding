import time

# Function to generate combinations using recursive backtracking
def generate_combinations(char_set, max_length, password, current_string, guesses):
    if current_string == password:
        elapsed_time = time.time() - generate_combinations.start_time
        print(f"Found {password} with {guesses} guesses in {elapsed_time:.6f} seconds")
        exit(0)

    for c in char_set:
        current_string += c
        guesses += 1

        if len(current_string) <= max_length:
            generate_combinations(char_set, max_length, password, current_string, guesses)

        current_string = current_string[:-1]

def main():
    char_set = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()-_=+[]{}|,'.<>?~"

    password = input("Password: ")
    max_length = int(input("Length: "))

    current_string = ""
    guesses = 0

    generate_combinations.start_time = time.time()
    generate_combinations(char_set, max_length, password, current_string, guesses)

if __name__ == "__main__":
    main()
