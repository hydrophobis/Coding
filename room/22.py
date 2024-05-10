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
        time.sleep(0.01)

if __name__ == "__main__":
    main()
