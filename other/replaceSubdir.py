import os

# Specify the directory where you want to perform the replacements
directory = os.path.expanduser("~/../workspaces/TestingWithCPP/c#/Xenon/")
print(directory)

# The text pattern to find and the text to replace it with
text_to_find = "XenonCheatEngine"
text_to_replace = "XenonCheatEngine"

# Get the absolute path of the script file
script_file_path = os.path.abspath(__file__)
print(script_file_path)

# Traverse through all files in the specified directory and its subdirectories
for root, _, files in os.walk(directory):
    print("d1")
    for file in files:
        file_path = os.path.join(root, file)
        print(file_path)

        # Check if the file is the script file itself, if so, skip it
        if file_path == script_file_path:
            continue

        # Replace text in each file
        with open(file_path, 'rb') as f:
            content = f.read()

        # Perform replacement in bytes
        new_content = content.replace(text_to_find.encode(), text_to_replace.encode())

        with open(file_path, 'wb') as f:
            f.write(new_content)