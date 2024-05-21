import subprocess
import time
from datetime import datetime

# Whenever this script detects a ping to port 21 it will close port 22 for a few seconds making it appear invisible to the scanner

# Paths to necessary commands and files
IPTABLES = "/sbin/iptables"
FTP_LOG = "/var/log/ftp_monitor.log"
BLOCK_SCRIPT = "block22.sh"

def log_ftp_connection():
    # Log the detected FTP connection
    with open(FTP_LOG, "a") as log_file:
        log_file.write(f"{datetime.now()}: FTP connection detected\n")

def block_port_22():
    # Execute the block script
    subprocess.run(["bash", BLOCK_SCRIPT])

def monitor_ftp_connections():
    # Run tcpdump to monitor port 21 connections
    tcpdump_process = subprocess.Popen(
        ["tcpdump", "-n", "-p", "tcp", "port", "21", "-l"],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )

    try:
        # Read the output line by line
        for line in iter(tcpdump_process.stdout.readline, ''):
            if line:
                log_ftp_connection()
                block_port_22()
    except KeyboardInterrupt:
        print("Stopping the monitoring script.")
        tcpdump_process.terminate()
    finally:
        tcpdump_process.stdout.close()
        tcpdump_process.wait()

def main():
    while True:
        monitor_ftp_connections()
        time.sleep(0.01)

if __name__ == "__main__":
    main()
