#include <iostream>
#include <string>
#include <stdexcept>
#include <thread>
#include <atomic>
#include <chrono>

#include "CanaryMonitor.h"
#include "FileCanary.h"
#include "FolderCanary.h"
#include "NetworkCanary.h"

// ============================================================
// Canary Forensic Detection System
// OOP Project — C++
// ============================================================

// This flag is shared between the main thread and the auto-scan thread.
// When the user presses Enter to stop, we set this to false
// and the scanning thread exits cleanly.
std::atomic<bool> autoScanRunning(false);  // atomic = thread-safe boolean

// autoScanLoop()
// This function runs on a SEPARATE THREAD.
// It keeps scanning every N seconds until autoScanRunning becomes false.
void autoScanLoop(CanaryMonitor* monitor, int intervalSeconds) {
    std::cout << "\n  [AUTO-SCAN] Started. Scanning every "
              << intervalSeconds << " seconds.\n";
    std::cout << "  [AUTO-SCAN] Press Enter to stop...\n\n";

    while (autoScanRunning) {
        monitor->scanAll();

        // Wait for intervalSeconds, but check every 1 second if we should stop.
        // This way the program stops quickly when user presses Enter.
        for (int i = 0; i < intervalSeconds && autoScanRunning; i++) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    std::cout << "\n  [AUTO-SCAN] Stopped.\n";
}

void printBanner() {
    std::cout << "\n";
    std::cout << "  =============================================\n";
    std::cout << "       CANARY FORENSIC DETECTION SYSTEM       \n";
    std::cout << "  =============================================\n";
    std::cout << "   Detect intruders using digital canary traps\n";
    std::cout << "  =============================================\n";
}

void printMenu() {
    std::cout << "\n";
    std::cout << "  +--------------------------------+\n";
    std::cout << "  |           MAIN MENU            |\n";
    std::cout << "  +--------------------------------+\n";
    std::cout << "  | 1. Deploy File Canary          |\n";
    std::cout << "  | 2. Deploy Folder Canary        |\n";
    std::cout << "  | 3. Deploy Network Canary       |\n";
    std::cout << "  | 4. Scan All Canaries (Manual)  |\n";
    std::cout << "  | 5. Start Auto-Scan             |\n";
    std::cout << "  | 6. View Deployed Canaries      |\n";
    std::cout << "  | 7. View Alert Log              |\n";
    std::cout << "  | 8. Simulate Network Attack     |\n";
    std::cout << "  | 0. Exit                        |\n";
    std::cout << "  +--------------------------------+\n";
    std::cout << "  Choice: ";
}

int main() {

    printBanner();

    CanaryMonitor  monitor;
    NetworkCanary* lastNetworkCanary = nullptr;

    int choice;

    do {
        printMenu();
        std::cin >> choice;
        std::cin.ignore();

        std::cout << "\n";

        try {

            // ---- Deploy File Canary ----
            if (choice == 1) {
                std::string name, path;

                std::cout << "  Enter canary name (e.g. PasswordTrap): ";
                std::getline(std::cin, name);

                std::cout << "  Enter full file path (e.g. C:/CanaryTrap/Passwords.txt): ";
                std::getline(std::cin, path);

                std::string bait =
                    "=== CONFIDENTIAL ===\n"
                    "admin      : admin@1234\n"
                    "root       : toor2024\n"
                    "db_user    : database#99\n"
                    "backup_key : XK29-PLMQ-9821\n"
                    "====================\n";

                monitor.addCanary(new FileCanary(name, path, bait));

            // ---- Deploy Folder Canary ----
            } else if (choice == 2) {
                std::string name, path;

                std::cout << "  Enter canary name (e.g. ConfidentialFolder): ";
                std::getline(std::cin, name);

                std::cout << "  Enter folder path (e.g. C:/CanaryTrap/Confidential): ";
                std::getline(std::cin, path);

                monitor.addCanary(new FolderCanary(name, path));

            // ---- Deploy Network Canary ----
            } else if (choice == 3) {
                std::string name, logPath;
                int port;

                std::cout << "  Enter canary name (e.g. FakeSSH): ";
                std::getline(std::cin, name);

                std::cout << "  Enter fake port number (e.g. 22, 3389, 8080): ";
                std::cin >> port;
                std::cin.ignore();

                std::cout << "  Enter log file path (e.g. C:/CanaryTrap/network_log.txt): ";
                std::getline(std::cin, logPath);

                lastNetworkCanary = new NetworkCanary(name, logPath, port);
                monitor.addCanary(lastNetworkCanary);

            // ---- Manual Scan ----
            } else if (choice == 4) {
                monitor.scanAll();

            // ---- Auto Scan ----
            } else if (choice == 5) {

                if (monitor.getTotalCanaries() == 0) {
                    std::cout << "  Please deploy at least one canary first.\n";

                } else {
                    int interval;
                    std::cout << "  Scan every how many seconds? (e.g. 5): ";
                    std::cin >> interval;
                    std::cin.ignore();

                    if (interval < 1) interval = 1;  // minimum 1 second

                    // Start auto-scan on a background thread
                    autoScanRunning = true;
                    std::thread scanThread(autoScanLoop, &monitor, interval);

                    // Main thread just waits for user to press Enter
                    std::cin.get();

                    // Tell the scan thread to stop and wait for it to finish
                    autoScanRunning = false;
                    scanThread.join();
                }

            // ---- Show Deployed Canaries ----
            } else if (choice == 6) {
                monitor.showStatus();

            // ---- View Alert Log ----
            } else if (choice == 7) {
                monitor.showAlerts();

            // ---- Simulate Network Attack ----
            } else if (choice == 8) {
                if (lastNetworkCanary == nullptr) {
                    std::cout << "  Please deploy a Network Canary first (option 3).\n";
                } else {
                    std::cout << "  Simulating a hacker connecting to the fake port...\n";
                    lastNetworkCanary->simulateAttack();
                    std::cout << "  Now run Scan to see the alert!\n";
                }

            } else if (choice != 0) {
                std::cout << "  Invalid choice. Please enter a valid option.\n";
            }

        } catch (std::exception& e) {
            std::cout << "  [ERROR] " << e.what() << "\n";
        }

    } while (choice != 0);

    // If user exits while auto-scan is running, stop it cleanly
    if (autoScanRunning) {
        autoScanRunning = false;
    }

    std::cout << "\n  Shutting down Canary System. Goodbye.\n\n";
    return 0;
}
