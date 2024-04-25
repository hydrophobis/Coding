import os

# Specify the input file path
input_file_path = "mcgraw.txt"

# Specify the output file path for unique lines
output_file_path = "mcgrawf.txt"

# Create a set to track unique lines
unique_lines = set()

# Read the input file and store unique lines in the set
with open(input_file_path, 'r') as input_file:
    for line in input_file:
        # Remove leading/trailing whitespace (e.g., newlines, spaces)
        cleaned_line = line.strip()

        # Add to the set if it's not already there
        if cleaned_line not in unique_lines:
            unique_lines.add(cleaned_line)

# Write the unique lines to the output file
with open(output_file_path, 'w') as output_file:
    for line in unique_lines:
        # Write the line followed by a newline character
        output_file.write(line + "\n")

print(f"Unique lines have been written to {output_file_path}.")
