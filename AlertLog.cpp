#include "AlertLog.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <stdexcept>

// Initialize static member (must be done outside the class)
int AlertLog::totalAlerts = 0;

// Constructor
AlertLog::AlertLog(std::string path) {
    this->logFilePath = path;
}

// Destructor
AlertLog::~AlertLog() {}

// writeAlert()
// Appends a formatted alert entry to the log file.
// Each alert has a number, timestamp, canary name, type, and path.
void AlertLog::writeAlert(const std::string& canaryName,
                          const std::string& canaryType,
                          const std::string& canaryPath) {

    std::ofstream logFile(logFilePath, std::ios::app);  // append so we don't erase old alerts

    if (!logFile.is_open()) {
        throw std::runtime_error("Cannot open alert log file: " + logFilePath);
    }

    // Get current time as a readable string
    std::time_t now = std::time(0);
    std::string timestamp = std::string(ctime(&now));
    timestamp.pop_back();  // remove trailing newline character

    totalAlerts++;  // increment static counter (Week 4)

    logFile << "============================================\n";
    logFile << " ALERT #" << totalAlerts << "\n";
    logFile << "============================================\n";
    logFile << " Time    : " << timestamp  << "\n";
    logFile << " Canary  : " << canaryName << "\n";
    logFile << " Type    : " << canaryType << "\n";
    logFile << " Path    : " << canaryPath << "\n";
    logFile << " Status  : INTRUSION DETECTED\n";
    logFile << "--------------------------------------------\n\n";

    logFile.close();
}

// showAllAlerts()
// Reads alerts.txt and prints its contents to the console.
void AlertLog::showAllAlerts() const {
    std::ifstream logFile(logFilePath);

    if (!logFile.is_open()) {
        std::cout << "  No alert log found. No alerts have been triggered yet.\n";
        return;
    }

    std::string line;
    while (std::getline(logFile, line)) {
        std::cout << line << "\n";
    }
    logFile.close();
}

// clearLog()
// Wipes the alert log (used for fresh start)
void AlertLog::clearLog() {
    std::ofstream logFile(logFilePath, std::ios::trunc);
    logFile.close();
    totalAlerts = 0;
    std::cout << "  Alert log cleared.\n";
}

// getTotalAlerts() — static function (Week 4)
int AlertLog::getTotalAlerts() {
    return totalAlerts;
}
