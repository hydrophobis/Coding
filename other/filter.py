import argparse

# Function to extract lines with a given term and additional context lines
def extract_lines_with_context(filepath, search_term, context_lines=2):
    """
    Extracts lines containing a specified term along with surrounding context lines.

    :param filepath: Path to the text file to process.
    :param search_term: Term to search for in the lines.
    :param context_lines: Number of surrounding lines to include for context.
    """
    with open(filepath, 'r') as file:
        lines = file.readlines()  # Read all lines in the file

    # A list to store the indices of lines containing the search term
    matching_indices = []

    # Identify the indices of lines containing the search term
    for index, line in enumerate(lines):
        if search_term.lower() in line.lower():  # Case-insensitive search
            matching_indices.append(index)

    # A list to store groups of lines with context
    grouped_lines = []

    # Add the context lines along with the lines containing the search term
    for index in matching_indices:
        start = max(0, index - context_lines)  # Start index for context
        end = min(len(lines), index + context_lines + 1)  # End index for context
        # Capture the context lines for this matching index
        context_group = lines[start:end]
        grouped_lines.append(context_group)

    return grouped_lines


# Main script
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Extract lines with a specified term and context.')
    parser.add_argument('input_file', type=str, help='Path to the text file to process.')
    parser.add_argument('term', type=str, help='Term to search for.')
    parser.add_argument('output_file', type=str, help='Path to the output text file.')
    parser.add_argument('--context', type=int, default=2, help='Number of surrounding context lines.')
    
    args = parser.parse_args()

    # Extract the groups of lines with context
    grouped_lines = extract_lines_with_context(args.input_file, args.term, args.context)

    # Write the extracted lines to the output file with extra newline separation
    with open(args.output_file, 'w') as output:
        for group in grouped_lines:
            for line in group:
                output.write(line.strip() + '\n')  # Write each line
            output.write('\n')  # Extra newline to separate groups
