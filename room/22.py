import subprocess
import time

def start_target_file():
    try:
        subprocess.run(["./mon21.sh"], check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error: {e}")
    except KeyboardInterrupt:
        print("Stopping the script.")
        return

def main():
    while True:
        start_target_file()
        # Adjust the delay time according to your needs
        time.sleep(2)  # Repeat every 5 seconds

if __name__ == "__main__":
    main()
