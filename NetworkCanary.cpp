#include "NetworkCanary.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <ctime>

// Constructor
NetworkCanary::NetworkCanary(std::string name, std::string path, int port)
    : Canary(name, path) {
    this->fakePort         = port;
    this->originalLineCount = 0;
}

// Destructor
NetworkCanary::~NetworkCanary() {}

// deployCanary()
// Creates a fake server log file with some starting entries.
// The log makes it look like a real service is running.
void NetworkCanary::deployCanary() {

    std::ofstream logFile(path);
    if (!logFile.is_open()) {
        throw std::runtime_error("Could not create network canary log at: " + path);
    }

    // Write fake server startup lines (the bait)
    logFile << "[SERVER] Fake SSH Service started on port " << fakePort << "\n";
    logFile << "[SERVER] Listening for connections...\n";
    logFile << "[SERVER] Status: ACTIVE\n";
    logFile.close();

    originalLineCount = countLogLines();
    triggered         = false;

    std::cout << "  [Deployed] " << name << " -> Port " << fakePort
              << " log at " << path << "\n";
}

// checkStatus()
// If the log file has more lines than when we deployed,
// it means someone (or something) wrote to it — a connection was made.
bool NetworkCanary::checkStatus() {
    int currentLines = countLogLines();

    if (currentLines == -1) {
        triggered = true;
        return true;  // log file deleted — suspicious!
    }

    if (currentLines > originalLineCount) {
        triggered = true;
        return true;  // new connection detected!
    }

    return false;  // no new connections, all safe
}

// getType()
std::string NetworkCanary::getType() const {
    return "NetworkCanary";
}

// simulateAttack()
// Writes a fake connection entry into the log.
// Used during demo to show the canary being triggered
// (simulates Member 2 connecting to the fake port).
void NetworkCanary::simulateAttack() {
    std::ofstream logFile(path, std::ios::app);  // append mode
    if (!logFile.is_open()) {
        throw std::runtime_error("Cannot write to network log: " + path);
    }

    std::time_t now = std::time(0);
    std::string timestamp = std::string(ctime(&now));
    timestamp.pop_back();  // remove trailing newline

    logFile << "[CONNECTION] Incoming connection at " << timestamp
            << " from 192.168.1.101 on port " << fakePort << "\n";
    logFile.close();

    std::cout << "  [Simulated] A fake connection was made to port " << fakePort << "\n";
}

// ---- Private Helper ----

// countLogLines()
// Opens the log file and counts how many lines it has.
int NetworkCanary::countLogLines() {
    std::ifstream logFile(path);
    if (!logFile.is_open()) return -1;

    int    count = 0;
    std::string line;
    while (std::getline(logFile, line)) {
        count++;
    }
    logFile.close();
    return count;
}
