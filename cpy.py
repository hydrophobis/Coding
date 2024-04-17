import ast
import subprocess
import sys

def getContent(filename):
    try:
        with open(filename, 'r') as file:
            content = file.read()
        return content
    except FileNotFoundError:
        print(f"File '{filename}' not found.")
        return None


def runFile(filename):
    subprocess.run(["python", filename])

def write(filename, content):
    try:
        with open(filename, 'w') as file:
            file.write(content)
        print(f"Content successfully written, running parsed content...\n")
    except Exception as e:
        print(f"Error writing to '{filename}': {e}")


def translate(content):
    content = content.replace('{', ':').replace('}', '').replace(';', '\n')
    content = content.replace('int main()', 'if __name__ == "__main__"')
    return content  # Return the translated content

def main():
    if len(sys.argv) != 2:
        print("Usage: cpy <filename>")
        sys.exit(1)

    filename = sys.argv[1]
    code = getContent(filename)
    if code is not None:  # Check if content is retrieved successfully
        code = translate(code)
        write('cpyrunner.py', code)
        runFile("cpyrunner.py")
    else:
        print("Exiting due to missing file.")

if __name__ == "__main__":
    main()
