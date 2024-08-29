import time

def generate_combinations(char_set, max_length, password):
    char_set_len = len(char_set)
    current_combination = [0] * max_length
    guesses = 0

    start_time = time.time()

    while True:
        guesses += 1
        # Generate the current string from current_combination
        current_string = ''.join(char_set[i] for i in current_combination)


        if current_string == password:
            elapsed_time = time.time() - start_time
            print(f"Found {password} with {guesses} guesses in {elapsed_time:.6f} seconds")
            return

        # Increment the combination
        pos = max_length - 1
        while pos >= 0:
            current_combination[pos] += 1
            if current_combination[pos] == char_set_len:
                current_combination[pos] = 0
                pos -= 1
            else:
                break

if __name__ == "__main__":
    char_set = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()-_=+[]{}|,'.<>?~"
    password = input("Password: ")
    max_length = int(input("Length: "))

    generate_combinations(char_set, max_length, password)
