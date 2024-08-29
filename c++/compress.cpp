#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <algorithm>

std::string compress(const std::string& data, std::unordered_map<char, std::string>& encoding) {
    // Step 1: Frequency Analysis
    std::unordered_map<char, int> frequency;
    for (char c : data) {
        frequency[c]++;
    }

    // Step 2: Sort by frequency and assign the smallest binary codes to most frequent elements
    std::vector<std::pair<char, int>> sorted_items(frequency.begin(), frequency.end());
    std::sort(sorted_items.begin(), sorted_items.end(), [](auto& a, auto& b) {
        return b.second < a.second;
    });

    int code = 0;
    for (const auto& item : sorted_items) {
        encoding[item.first] = std::to_string(code); // use the integer as the code directly
        code++;
    }

    // Step 3: Encode the data
    std::stringstream compressed_data;
    for (char c : data) {
        compressed_data << encoding[c];
        if (encoding[c].length() == 2) {
            compressed_data << ","; // add a comma only after two-digit values
        }
    }

    std::string result = compressed_data.str();
    if (result.back() == ',') {
        result.pop_back(); // Remove trailing comma
    }

    return result;
}

std::string decompress(const std::string& compressed_data, const std::unordered_map<char, std::string>& encoding) {
    // Reverse the encoding map to create a decoding map
    std::unordered_map<std::string, char> decoding;
    for (const auto& pair : encoding) {
        decoding[pair.second] = pair.first;
    }

    std::string decompressed_data;
    std::stringstream ss(compressed_data);
    std::string token;
    
    // This will handle single and two-digit values correctly
    while (std::getline(ss, token, ',')) {
        // Retrieve the original character for the decoded token
        if (decoding.find(token) != decoding.end()) {
            decompressed_data += decoding.at(token);
        } else {
            std::cerr << "Error: Unknown token " << token << " during decompression.\n";
        }
    }

    return decompressed_data;
}

int main() {
    std::string data = "1234";
    std::unordered_map<char, std::string> encoding;

    std::string compressed_data = compress(data, encoding);
    std::cout << "Compressed Data: " << compressed_data << std::endl;

    std::cout << "Encoding:" << std::endl;
    for (const auto& pair : encoding) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    std::string decompressed_data = decompress(compressed_data, encoding);
    std::cout << "Decompressed Data: " << decompressed_data << std::endl;

    return 0;
}
