#!/bin/bash
BACKUP_DIR=$1
BACKUP_NAME=$2

# Create backup
tar -czf "$BACKUP_NAME.tar.gz" "$BACKUP_DIR"