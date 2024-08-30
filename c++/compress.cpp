#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>

const std::string BASE62_ALPHABET = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

// Encode an integer as a base-62 string
std::string encodeBase62(int value) {
    static const char* base62Chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string encoded;
    do {
        encoded = base62Chars[value % 62] + encoded;
        value /= 62;
    } while (value > 0);
    return encoded.empty() ? "0" : encoded;
}

// Decode a base-62 string to an integer
int decodeBase62(const std::string& str) {
    int result = 0;
    for (char c : str) {
        result = result * 62 + BASE62_ALPHABET.find(c);
    }
    return result;
}

std::string compress(const std::string& data, std::unordered_map<char, std::string>& encoding) {
    std::unordered_map<char, int> frequency;
    for (char c : data) {
        frequency[c]++;
    }

    std::vector<std::pair<char, int>> sorted_items(frequency.begin(), frequency.end());
    std::sort(sorted_items.begin(), sorted_items.end(), [](const std::pair<char, int>& a, const std::pair<char, int>& b) {
        return a.second > b.second;
    });

    int code = 0;
    for (const auto& item : sorted_items) {
        encoding[item.first] = encodeBase62(code);
        code++;
    }

    std::stringstream compressed_data;
    for (char c : data) {
        compressed_data << encoding[c];
    }

    std::stringstream encoding_table;
    for (const auto& pair : encoding) {
        encoding_table << pair.second << ":" << pair.first << ";";
    }

    return compressed_data.str() + "\1" + encoding_table.str();
}


std::unordered_map<std::string, char> parseDecodingTable(const std::string& table) {
    std::unordered_map<std::string, char> decoding;
    std::stringstream ss(table);
    std::string entry;

    while (std::getline(ss, entry, ';')) {
        if (!entry.empty()) {
            size_t pos = entry.find(':');
            if (pos != std::string::npos) {
                std::string code = entry.substr(0, pos);
                char character = entry[pos + 1];
                decoding[code] = character;
            } else {
                std::cerr << "Error: Invalid entry format in encoding table: " << entry << "\n";
            }
        }
    }

    return decoding;
}

std::string decompress(const std::string& compressed_data, const std::unordered_map<std::string, char>& decoding) {
    std::string decompressed_data;
    size_t pos = 0;

    while (pos < compressed_data.length()) {
        bool found = false;

        // Try to find the longest matching token
        for (size_t length = 1; length <= compressed_data.length() - pos; ++length) {
            std::string token = compressed_data.substr(pos, length);
            auto it = decoding.find(token);

            if (it != decoding.end()) {
                decompressed_data += it->second;
                pos += length;
                found = true;
                break;
            }
        }

        // If no token was found, print an error and stop
        if (!found) {
            std::cerr << "Error: Unknown token \"" << compressed_data.substr(pos, compressed_data.length() - pos) << "\" during decompression.\n";
            return "";
        }
    }

    return decompressed_data;
}


void printUsage() {
    std::cerr << "Usage: ./compressor [-c|-d] -i input_file -o output_file\n";
    std::cerr << "  -c           Compress input file\n";
    std::cerr << "  -d           Decompress input file\n";
    std::cerr << "  -i input_file  Specify input file\n";
    std::cerr << "  -o output_file Specify output file\n";
}

std::string compressToInteger(const std::string& compressed_data) {
    unsigned long long number = 0;
    for (char c : compressed_data) {
        number = number * 62 + BASE62_ALPHABET.find(c);
    }
    return std::to_string(number);
}


// Convert an integer back to the base-62 encoded string
std::string decompressFromInteger(const std::string& number_str) {
    unsigned long long number = std::stoull(number_str);
    std::string result;
    while (number > 0) {
        result += BASE62_ALPHABET[number % 62];
        number /= 62;
    }
    std::reverse(result.begin(), result.end());
    return result.empty() ? "0" : result;
}

int main(int argc, char** argv) {
    if (argc < 5) {
        printUsage();
        return 1;
    }

    bool compressMode = false;
    bool decompressMode = false;
    std::string inputFileName;
    std::string outputFileName;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-c") {
            compressMode = true;
        } else if (arg == "-d") {
            decompressMode = true;
        } else if (arg == "-i" && i + 1 < argc) {
            inputFileName = argv[++i];
        } else if (arg == "-o" && i + 1 < argc) {
            outputFileName = argv[++i];
        } else {
            printUsage();
            return 1;
        }
    }

    if (!compressMode && !decompressMode) {
        printUsage();
        return 1;
    }

    if (inputFileName.empty()) {
        printUsage();
        return 1;
    }

    if (outputFileName.empty()) {
        std::cout << "No output file name, decompressing to the same filename\n";
        outputFileName = inputFileName + ".cmp";
    }

    std::ifstream inputFile(inputFileName);
    if (!inputFile) {
        std::cerr << "Error: Could not open input file.\n";
        return 1;
    }

    std::ofstream outputFile(outputFileName);
    if (!outputFile) {
        std::cerr << "Error: Could not open output file.\n";
        return 1;
    }

    std::string data((std::istreambuf_iterator<char>(inputFile)),
                     std::istreambuf_iterator<char>());

    std::unordered_map<char, std::string> encoding;
    std::unordered_map<std::string, char> decoding;
    std::string result;

    if (compressMode) {
        result = compress(data, encoding);
        std::string integer_compressed = compressToInteger(result);

        std::stringstream encoding_table;
        for (const auto& pair : encoding) {
            encoding_table << pair.second << ":" << pair.first << ";";
        }

        outputFile << integer_compressed + "\1" + encoding_table.str();
    } else if (decompressMode) {
        size_t separator_pos = data.find('\1');
        if (separator_pos == std::string::npos) {
            std::cerr << "Error: Invalid format. Missing separator.\n";
            return 1;
        }

        std::string compressed_part = data.substr(0, separator_pos);
        std::string table_part = data.substr(separator_pos + 1);

        std::cout << "Compressed Part (as integer): " << compressed_part << "\n";
        std::cout << "Table Part: " << table_part << "\n";

        std::string decompressed_data_as_string = decompressFromInteger(compressed_part);
        decoding = parseDecodingTable(table_part);

        std::cout << "Decompressed Data as String: " << decompressed_data_as_string << "\n";

        result = decompress(decompressed_data_as_string, decoding);
        if (result.empty()) {
            std::cerr << "Error: Decompression failed.\n";
            return 1;
        }
        outputFile << result;
    }

    return 0;
}
