#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <Windows.h>
#include <Shlobj.h>



int main() {

    std::string filePath = "C:/Program Files (x86)/Steam/steamapps/common/Redmatch 2/GameAssembly.dll";

    int g;
    std::cout << "Your steam path to GameAssembly.dll is " << filePath << " ?\n 0 - no, 1 - yes\n";
    int nn;
    std::cin >> nn;
    if (nn == 0) {
        std::cout << "Enter your steam path to GameAssembly.dll like in example:\n";
        getline(std::cin, filePath);
        getline(std::cin, filePath);
    }
    std::ifstream file(filePath, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Wrong openning file\n";
        return 1;
    }

    std::string signatureString = "40 55 48 83 ec ?? 48 8d 6c 24 ?? 48 89 5d ?? 48 89 75 ?? 48 89 7d ?? 48 8b f1 80 3d ?? ?? ?? ?? ?? 75 ?? 8b 0d ?? ?? ?? ?? e8 ?? ?? ?? ?? c6 05 ?? ?? ?? ?? ?? 8b 04 24 48 83 ec ?? 8b 04 24 48 8b 0d";
    std::vector<char> signature;

    std::istringstream ss(signatureString);
    std::string byteString;
    while (std::getline(ss, byteString, ' ')) {
        if (byteString == "??") {
            signature.push_back('\x00');
        }
        else {
            signature.push_back(std::stoi(byteString, nullptr, 16));
        }
    }
    std::vector<char> buffer(std::istreambuf_iterator<char>(file), {});
    auto result = buffer.begin();
    while (result != buffer.end()) {
        result = std::find(result, buffer.end(), signature[0]);
        if (result == buffer.end()) {
            break;
        }
        bool match = true;
        for (size_t i = 1; i < signature.size(); i++) {
            if (signature[i] != '\x00' && *(result + i) != signature[i]) {
                match = false;
                break;
            }
        }
        if (match) {
            std::streampos offset = result - buffer.begin();
            std::fstream file2(filePath, std::ios::in | std::ios::out | std::ios::binary);
            file2.seekp(offset);
            file2.put(0xC3);
            file2.put(0x90);
            std::cout << "Found";
            return 0;
        }
        result++;
    }
    return 1;
}