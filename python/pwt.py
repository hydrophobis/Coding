import threading
import time
import os

# Global variables
found = False
found_lock = threading.Lock()

# Function to generate combinations using recursive backtracking
def generate_combinations(char_set, max_length, password, current_string, guesses):
    global found
    if found:
        return

    if len(current_string) == max_length:
        return

    for c in char_set:
        if found:
            return

        current_string += c
        guesses[0] += 1

        if current_string == password:
            with found_lock:
                if not found:  # Double-check in case another thread found it first
                    found = True
                    print(f"Found {password} with {guesses[0]} guesses")
            return

        generate_combinations(char_set, max_length, password, current_string, guesses)
        current_string = current_string[:-1]

def worker(char_set, max_length, password, guesses):
    generate_combinations(char_set, max_length, password, "", guesses)

def main():
    global found

    char_set = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()-_=+[]{}|,'.<>?~"

    password = input("Password: ")
    max_length = int(input("Length: "))

    start = time.time()

    num_threads = min(32, os.cpu_count())  # Number of threads
    threads = []
    guesses = [0]  # Shared list to keep track of guesses

    for _ in range(num_threads):
        t = threading.Thread(target=worker, args=(char_set, max_length, password, guesses))
        threads.append(t)
        t.start()

    for t in threads:
        t.join()

    elapsed_time = time.time() - start
    print(f"Time elapsed: {elapsed_time:.6f}s")

if __name__ == "__main__":
    main()
