#include "CanaryMonitor.h"
#include <iostream>

// Constructor — AlertLog will automatically use "alerts.txt"
CanaryMonitor::CanaryMonitor() : alertLog("alerts.txt") {}

// Destructor — deletes all dynamically allocated canary objects
// This prevents memory leaks (good memory management practice)
CanaryMonitor::~CanaryMonitor() {
    for (Canary* c : canaries) {
        delete c;
    }
    canaries.clear();
}

// addCanary()
// Deploys the canary (creates the trap) and adds it to our list.
void CanaryMonitor::addCanary(Canary* canary) {
    canary->deployCanary();          // place the trap
    canaries.push_back(canary);      // remember it
}

// scanAll()
// This is where POLYMORPHISM happens (Week 10).
// We loop through Canary* pointers and call checkStatus().
// C++ automatically calls the correct version:
//   → FileCanary::checkStatus()   for file canaries
//   → FolderCanary::checkStatus() for folder canaries
//   → NetworkCanary::checkStatus() for network canaries
void CanaryMonitor::scanAll() {

    if (canaries.empty()) {
        std::cout << "\n  No canaries deployed yet.\n";
        return;
    }

    std::cout << "\n  Scanning " << canaries.size() << " canary(s)...\n";
    std::cout << "  ----------------------------------\n";

    for (Canary* c : canaries) {

        bool wasTriggered = c->checkStatus();  // polymorphic call

        if (wasTriggered) {
            std::cout << "  [!!! ALERT] " << c->getName()
                      << " (" << c->getType() << ") TRIGGERED!\n";

            // Log the alert permanently to alerts.txt
            alertLog.writeAlert(c->getName(), c->getType(), c->getPath());

        } else {
            std::cout << "  [  SAFE  ] " << c->getName()
                      << " (" << c->getType() << ") - No changes.\n";
        }
    }

    std::cout << "  ----------------------------------\n";
    std::cout << "  Total alerts so far: " << AlertLog::getTotalAlerts() << "\n";
}

// showStatus()
// Prints a list of all deployed canaries and their current state.
void CanaryMonitor::showStatus() const {

    if (canaries.empty()) {
        std::cout << "\n  No canaries deployed yet.\n";
        return;
    }

    std::cout << "\n  ---- Deployed Canaries ----\n";
    for (int i = 0; i < (int)canaries.size(); i++) {
        std::cout << "  [" << i + 1 << "] "
                  << canaries[i]->getName()
                  << " | Type: " << canaries[i]->getType()
                  << " | Path: " << canaries[i]->getPath()
                  << " | Status: " << (canaries[i]->isTriggered() ? "TRIGGERED" : "Safe")
                  << "\n";
    }
}

// showAlerts()
// Reads and displays everything from alerts.txt
void CanaryMonitor::showAlerts() const {
    std::cout << "\n  ---- Alert Log (alerts.txt) ----\n";
    alertLog.showAllAlerts();
}

// getTotalCanaries()
int CanaryMonitor::getTotalCanaries() const {
    return canaries.size();
}
