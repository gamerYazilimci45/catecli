#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {

    if (argv[1] == "-n"){
        std::string folderName = argv[2];
    
    
        try {
            std::string homePath = std::getenv("HOME");
            fs::path configPath = homePath + "/.config/catecli";
            if (!fs::exists(configPath)) {
                fs::create_directories(configPath);

            }
            if(!fs::exists(folderName)) {
                fs::create_directory(folderName);
                std::cout << "New Category Folder is Created: " << folderName << "\n";
        
            }
            else{
                std::cout << "Category is already exists!" << "\n";
            }
            std::string folderPath = fs::absolute(folderName).string();
            std::ofstream outFile(configPath / "categories.txt", std::ios::app);
            if (outFile.is_open()) {
                outFile << folderPath << "\n";
                outFile.close();
            } else {
                return 1;
            }
        }

        
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
            return 1;
        }
    }
    return 0;


    
}