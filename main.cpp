#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

std::string findCategoryPath(const fs::path& configPath, const std::string& folderName) {
    std::ifstream inFile(configPath / "categories.txt");
    if (inFile.is_open()) {
        std::string line;
        while (std::getline(inFile, line)) {
            std::istringstream ss(line);
            std::string name, path;
            if (std::getline(ss, name, ':') && std::getline(ss, path)) {
                name = name.substr(0, name.find_last_not_of(" \t") + 1);
                path = path.substr(0, path.find_last_not_of(" \t") + 1);

                if (name == folderName) {
                    return path;
                }
}

        }
    } else {
        std::cerr << "Error: Unable to open categories.txt for reading.\n";
    }
    return "";
}

bool moveFile(const std::string& filePath, const std::string& targetFolderPath) {
    try {
        fs::path sourceFile = filePath;
        if (!fs::exists(sourceFile)) {
            std::cerr << "Error: File does not exist: " << filePath << "\n";
            return false;
        }

        fs::path targetFolder = targetFolderPath;
        if (!fs::exists(targetFolder)) {
            std::cerr << "Error: Target folder does not exist: " << targetFolderPath << "\n";
            return false;
        }

        fs::path targetFile = targetFolder / sourceFile.filename();
        fs::rename(sourceFile, targetFile);
        return true;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return false;
    }
}

bool copyFile(const std::string& filePath, const std::string& targetFolderPath) {
    try {
        fs::path sourceFile = filePath;
        if (!fs::exists(sourceFile)) {
            std::cerr << "Error: File does not exist: " << filePath << "\n";
            return false;
        }

        fs::path targetFolder = targetFolderPath;
        std::cout << "Target folder absolute path: " << targetFolder << "\n"; // Debugging

        if (!fs::exists(targetFolder)) {
            std::cerr << "Error: Target folder does not exist: " << targetFolderPath << "\n";
            return false;
        }

        if (!fs::is_directory(targetFolder)) {
            std::cerr << "Error: Target path is not a directory: " << targetFolderPath << "\n";
            return false;
        }

        fs::copy(sourceFile, targetFolder, fs::copy_options::overwrite_existing);
        std::cout << "File copied successfully.\n"; 
        return true;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return false;
    }
}


int main(int argc, char* argv[]) {

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

    if (std::string(argv[1]) == "-n") {
        std::string folderName = argv[2];

        try {
        

            fs::path folderPath = configPath / folderName;

            if (!fs::exists(configPath / folderName)) {
                fs::create_directory(configPath / folderName);
                std::cout << "New Category Folder is Created: " << folderName << "\n";
            } else {
                std::cout << "Category already exists!" << "\n";
            }

            std::ofstream outFile(configPath / "categories.txt", std::ios::app);
            if (outFile.is_open()) {
                outFile << folderName << " : " << folderPath << "\n";
            } else {
                std::cerr << "Error: Unable to open categories.txt for writing.\n";
                return 1;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
            return 1;
        }
    }
    else {
        std::string filePath = argv[1];
        std::string folderName = argv[2];
        std::string folderPath = findCategoryPath(configPath, folderName);

        if (argv[3]){
            if (std::string(argv[3]) == "-m") {
                if (!folderPath.empty() ){
                    if(!moveFile(filePath, folderPath)){
                        std::cerr << "Error: File could not be moved/copied.\n";
                        return 1;
                    }
                }
            
                else{
                    std::cerr << "Error: Folder '" << folderName << "' not found in categories.txt\n";
                    return 1;
                }
            }
        }



        if (!folderPath.empty() ){
            if(!copyFile(filePath, folderPath)){
                std::cerr << "Error: File could not be moved.\n";
                return 1;
            }
            else{
                std::cerr << "Error: Folder '" << folderName << "' not found in categories.txt\n";
                return 1;
            }
        }
    }
    return 0;
}