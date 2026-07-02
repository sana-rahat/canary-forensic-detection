#ifndef CANARYMONITOR_H
#define CANARYMONITOR_H

#include "Canary.h"
#include "AlertLog.h"
#include <vector>

// ============================================================
// CanaryMonitor — The Master Controller (Week 6 Composition, Week 10 Polymorphism)
// ============================================================
// This class manages all deployed canaries.
// It stores them as Canary* pointers (Week 5) which allows polymorphism —
// meaning it can call checkStatus() on a FileCanary, FolderCanary, or
// NetworkCanary without knowing which specific type it is.
//
// It also HAS-A AlertLog (Week 6: Composition) to record all intrusions.

class CanaryMonitor {

private:
    std::vector<Canary*>  canaries;   // Week 5: array of base class pointers
    AlertLog              alertLog;   // Week 6: Composition — monitor HAS-A log

public:

    // Constructor & Destructor
    CanaryMonitor();
    ~CanaryMonitor();  // deletes all canary objects to prevent memory leaks

    // Adds a new canary to the system and deploys it
    void addCanary(Canary* canary);

    // Scans ALL canaries — uses polymorphism to call correct checkStatus()
    void scanAll();

    // Prints a summary of all deployed canaries
    void showStatus() const;

    // View the full alert log
    void showAlerts() const;

    // Returns how many canaries are deployed
    int getTotalCanaries() const;
};

#endif
