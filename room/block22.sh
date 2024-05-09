#!/bin/bash

# Path to IPTables
IPTABLES="/sbin/iptables"

# Block port 22
$IPTABLES -A INPUT -p tcp --dport 22 -j DROP

# Wait for a specified timeout (e.g., 10 seconds)
sleep 10

# Unblock port 22
$IPTABLES -D INPUT -p tcp --dport 22 -j DROP
