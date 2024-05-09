# Find the PID of a process (example with `grep` for a specific process)
PID=$(pgrep process_name)

# Check if the PID exists and then kill it
if [ ! -z "$PID" ]; then
    kill $PID
    echo "Killed process with PID $PID"
else
    echo "No process found with the name $process_name"
fi
