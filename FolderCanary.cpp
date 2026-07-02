#include "FolderCanary.h"
#include <stdexcept>
#include <dirent.h>
#include <iostream>

// Constructor — calls parent constructor
FolderCanary::FolderCanary(std::string name, std::string path)
    : Canary(name, path) {
    this->originalFileCount = 0;
}

// Destructor
FolderCanary::~FolderCanary() {}

// deployCanary()
// Records how many files are currently in the folder as the baseline.
void FolderCanary::deployCanary() {
    originalFileCount = countFilesInFolder();

    if (originalFileCount == -1) {
        throw std::runtime_error("Folder not found: " + path);
    }

    triggered = false;
    std::cout << "  [Deployed] " << name << " -> " << path
              << " (" << originalFileCount << " files inside)\n";
}

// checkStatus()
// Counts files now and compares with the baseline count.
// Any difference means a file was added or removed.
bool FolderCanary::checkStatus() {
    int currentCount = countFilesInFolder();

    if (currentCount == -1) {
        // Folder itself was deleted — major intrusion!
        triggered = true;
        return true;
    }

    if (currentCount != originalFileCount) {
        triggered = true;
        return true;  // file was added or removed
    }

    return false;  // folder looks safe
}

// getType()
std::string FolderCanary::getType() const {
    return "FolderCanary";
}

// ---- Private Helper ----

// countFilesInFolder()
// Opens the folder using dirent.h and counts actual files inside.
// Returns -1 if the folder cannot be opened.
int FolderCanary::countFilesInFolder() {
    DIR* dir = opendir(path.c_str());

    if (dir == nullptr) {
        return -1;  // folder not accessible or doesn't exist
    }

    int count = 0;
    struct dirent* entry;

    while ((entry = readdir(dir)) != nullptr) {
        std::string entryName = entry->d_name;

        // Skip the special "." and ".." entries
        if (entryName == "." || entryName == "..") continue;

        count++;
    }

    closedir(dir);
    return count;
}
