#ifndef NETWORKCANARY_H
#define NETWORKCANARY_H

#include "Canary.h"

// ============================================================
// NetworkCanary — Derived Class (Week 9, 10)
// ============================================================
// This canary simulates a fake network service log.
// It creates a log file pretending to be a live server.
// If someone "connects" to it (we simulate this by adding a log entry),
// the system detects the new entry and raises an alert.

class NetworkCanary : public Canary {

private:
    int    fakePort;              // the fake port number e.g. 8080, 22, 3389
    int    originalLineCount;     // how many lines were in the log at deployment

    // Helper: counts lines in the fake service log file
    int countLogLines();

public:

    // Constructor
    NetworkCanary(std::string name, std::string path, int port);

    // Destructor
    ~NetworkCanary();

    // Override pure virtual functions (Week 10)
    void        deployCanary()           override;
    bool        checkStatus()            override;
    std::string getType()          const override;

    // Extra feature: lets us simulate a hacker connecting (for demo purposes)
    void simulateAttack();
};

#endif
