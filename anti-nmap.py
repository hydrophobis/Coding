import subprocess
import time
from datetime import datetime

# Paths to necessary commands and files
IPTABLES = "/sbin/iptables"
FTP_LOG = "/var/log/ftp_monitor.log"

def log_ftp_connection():
    # Log the detected FTP connection
    with open(FTP_LOG, "a") as log_file:
        log_file.write(f"{datetime.now()}: FTP connection detected\n")

def block_port_22():
    # Block port 22
    subprocess.run([IPTABLES, "-A", "INPUT", "-p", "tcp", "--dport", "22", "-j", "DROP"])
    print("Port 22 blocked.")
    time.sleep(10)  # Block for 10 seconds (adjust as needed)
    # Unblock port 22
    subprocess.run([IPTABLES, "-D", "INPUT", "-p", "tcp", "--dport", "22", "-j", "DROP"])
    print("Port 22 unblocked.")

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
