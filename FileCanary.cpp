#include "FileCanary.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <sys/stat.h>

// Constructor — calls parent constructor using initializer list
FileCanary::FileCanary(std::string name, std::string path, std::string baitContent)
    : Canary(name, path) {
    this->baitContent = baitContent;
}

// Destructor
FileCanary::~FileCanary() {}

// deployCanary()
// Creates the decoy file with fake bait content.
// Then immediately takes a baseline snapshot of the file's state.
void FileCanary::deployCanary() {

    // Write the bait file to disk
    std::ofstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not create canary file at: " + path);
    }
    file << baitContent;
    file.close();

    // Save baseline state — this is our "clean" reference point
    savedState  = captureCurrentState();
    triggered   = false;

    std::cout << "  [Deployed] " << name << " -> " << path << "\n";
}

// checkStatus()
// Takes a fresh snapshot and compares it with the saved baseline.
// Uses the overloaded != operator from CanaryState (Week 6).
// Returns true if tampering is detected.
bool FileCanary::checkStatus() {

    try {
        CanaryState currentState = captureCurrentState();

        // Using overloaded != operator here (Week 6)
        if (currentState != savedState) {
            triggered = true;
            return true;  // canary was triggered!
        }
        return false;     // file is safe, no changes

    } catch (std::exception& e) {
        // If file no longer exists — that itself is an intrusion (file was deleted)
        triggered = true;
        return true;
    }
}

// getType() — returns the class name as a string for display purposes
std::string FileCanary::getType() const {
    return "FileCanary";
}

// ---- Private Helper Functions ----

// computeChecksum()
// Reads the file and adds up the ASCII values of every character.
// Simple but effective for detecting any content change.
std::string FileCanary::computeChecksum(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot read file for checksum: " + filePath);
    }

    long long sum = 0;
    char ch;
    while (file.get(ch)) {
        sum += (unsigned char)ch;
    }
    file.close();

    return std::to_string(sum);
}

// captureCurrentState()
// Reads the file's size and modification time from the OS,
// then computes a checksum of the content.
// Returns a CanaryState snapshot.
CanaryState FileCanary::captureCurrentState() {
    struct stat fileStat;

    if (stat(path.c_str(), &fileStat) != 0) {
        throw std::runtime_error("File not found: " + path);
    }

    long long   size      = fileStat.st_size;
    std::time_t modified  = fileStat.st_mtime;
    std::string checksum  = computeChecksum(path);

    return CanaryState(size, checksum, modified);
}
