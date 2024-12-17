#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {

    if (std::string(argv[1]) == "-n") {
        std::string folderName = argv[2];

        try {
            const char* homeCStr = std::getenv("HOME");
            if (!homeCStr) {
                std::cerr << "Error: HOME environment variable is not set.\n";
                return 1;
            }
            std::string homePath = homeCStr;
            fs::path configPath = homePath + "/.config/catecli";

            if (!fs::exists(configPath)) {
                fs::create_directories(configPath);
            }

            fs::path folderPath = fs::absolute(folderName).string();

            if (!fs::exists(folderName)) {
                fs::create_directory(folderName);
                std::cout << "New Category Folder is Created: " << folderName << "\n";
            } else {
                std::cout << "Category already exists!" << "\n";
            }

            std::ofstream outFile(configPath / "categories.txt", std::ios::app);
            if (outFile.is_open()) {
                outFile << folderPath << "\n";
            } else {
                std::cerr << "Error: Unable to open categories.txt for writing.\n";
                return 1;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
            return 1;
        }
    return 0;
}