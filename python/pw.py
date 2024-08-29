import random
import time

def check_pw(pw, password):
    print(f"Guessing password: {pw}               Guess number: {check_pw.guesses}")
    return pw == password

def main():
    chars = ["a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", 
             "v", "w", "x", "y", "z", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", 
             "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "!", 
             "@", "#", "$", "%", "^", "&", "*", "(", ")", "-", "_", "=", "+", "[", "]", "{", "}", "|", ",", "'", "<", 
             ">", "/", "?", "~"]
    
    password = input("Password: ")
    pw_length = int(input("Password Length: "))
    check_pw.guesses = 0

    start_time = time.time()

    while True:
        current_pw = ''.join(random.choice(chars) for _ in range(pw_length))
        check_pw.guesses += 1

        if check_pw(current_pw, password):
            end_time = time.time()
            duration = end_time - start_time
            print(f"Guess successful: {password} in {check_pw.guesses} guesses, took {duration:.2f}s")
            break

if __name__ == "__main__":
    main()
