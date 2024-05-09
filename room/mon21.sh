#!/bin/bash

# Path to IPTables
IPTABLES="/sbin/iptables"

# Path to log file for monitoring port 21 connections
FTP_LOG="/var/log/ftp_monitor.log"

# Path to script that blocks/unblocks port 22
BLOCK_SCRIPT="block22.sh"

# Start monitoring for connections on port 21
# `-n` is for numeric output, `-p tcp` is for TCP, and `:21` targets port 21
tcpdump -n -p tcp port 21 -l | while read line; do
    # Log connection attempts
    echo "$(date): FTP connection detected" >> "$FTP_LOG"
    # Trigger blocking of port 22
    bash "$BLOCK_SCRIPT"
done
