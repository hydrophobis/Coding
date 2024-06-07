#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>


using namespace std;

struct Effect {
    string name;
    int id;
    int effectId;
};

struct Entity {
    string name;
    int health;
    int damage;
    int level;
    bool blocked;
};

struct Player : public Entity {

   vector<int> inventory;
   Item sword;
   Item shield;

    void addItem(int id, Player& player){
        player.inventory.push_back(id);
    }

    void levelUp(Player& player){
        player.level += 1;

        player.damage = (rand() % 5) * (player.health / player.level);

        prevHp = player.health;
        player.health = 0;

        while(player.health < prevHp){
            player.health = player.level * (rand() % 10);
        }
    }

    void useItem(int id, Player& player){
        vector<int> inv = player.inventory;
        find(inv.begin(), inv.end(), id);
    }
};

int totalItems = 8;

struct Item {
    string name;
    int id = NULL;
    int damage = 0;
    int block = 0;
    double weight = 0.0;
};

struct Food : public Item {
    int healAmount;
};

struct Game {

    Player player;
    vector<int> inventory;
    int playerHealth;
    int playerDamage;
    int playerLevel;
    string playerName;

}



// ChatGPT made this
class FileManager {
public:
    static const string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    static inline bool is_base64(unsigned char c) {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }

    string base64_decode(const string &encoded_string) {

        int in_len = encoded_string.size();
        int i = 0;
        int j = 0;
        int in_ = 0;
        unsigned char char_array_4[4], char_array_3[3];
        string ret;

        while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
            char_array_4[i++] = encoded_string[in_]; in_++;
            if (i == 4) {
                for (i = 0; i < 4; i++)
                    char_array_4[i] = base64_chars.find(char_array_4[i]);

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for (i = 0; (i < 3); i++)
                    ret += char_array_3[i];
                i = 0;
            }
        }

        if (i) {
            for (j = i; j < 4; j++)
                char_array_4[j] = 0;

            for (j = 0; j < 4; j++)
                char_array_4[j] = base64_chars.find(char_array_4[j]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
        }

        return ret;
    }
    string base64_encode(const string &input) {

        string encoded_string;
        int in_len = input.size();
        int i = 0;
        int j = 0;
        unsigned char char_array_3[3];
        unsigned char char_array_4[4];

        while (in_len--) {
            char_array_3[i++] = input[j++];
            if (i == 3) {
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;

                for (i = 0; (i < 4); i++)
                    encoded_string += base64_chars[char_array_4[i]];
                i = 0;
            }
        }

        if (i) {
            for (j = i; j < 3; j++)
                char_array_3[j] = '\0';

            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

            for (j = 0; (j < i + 1); j++)
                encoded_string += base64_chars[char_array_4[j]];

            while ((i++ < 3))
                encoded_string += '=';
        }

        return encoded_string;
    }

    void wipeFile(const string &fileName) {
        ofstream outputFile(fileName, ios::trunc);
        if (!outputFile) {
            throw runtime_error("Unable to open file");
        }
        // Opening the file in truncate mode clears its contents
    }

    void writeFile(const string &fileName, const string &content) {
        ofstream outputFile(fileName);
        if (!outputFile) {
            throw runtime_error("Unable to open file");
        }
        outputFile << content;
    }

    char** readFile(const string &fileName, int &lineCount) {
        ifstream inputFile(fileName);
        if (!inputFile) {
            throw runtime_error("Unable to open file");
        }

        vector<string> lines;
        string line;
        while (getline(inputFile, line)) {
            lines.push_back(line);
        }

        lineCount = lines.size();
        char** result = new char*[lineCount];

        for (int i = 0; i < lineCount; ++i) {
            result[i] = new char[lines[i].length() + 1];
            strcpy(result[i], lines[i].c_str());
        }

        return result;
    }

    void freeCharArray(char** arr, int lineCount) {
        for (int i = 0; i < lineCount; ++i) {
            delete[] arr[i];
        }
        delete[] arr;
    }
}

/*
    int woodItem = 0;
    int woodItem = 1;

    int stoneItem = 2;
    int stoneItem = 3;

    int ironItem = 4;
    int ironItem = 5;

    int goldItem = 6;
    int goldItem = 7;
*/