#ifndef FILECANARY_H
#define FILECANARY_H

#include "Canary.h"

// ============================================================
// FileCanary — Derived Class (Week 9, 10)
// ============================================================
// This canary monitors a single file.
// It creates a decoy file with fake sensitive content (the bait).
// If the file is opened, edited, or deleted — it raises an alert.

class FileCanary : public Canary {

private:
    std::string baitContent;    // fake content written inside the trap file

    // Helper functions (private — hidden from outside, Week 2)
    std::string computeChecksum(const std::string& filePath);
    CanaryState captureCurrentState();

public:

    // Constructor
    FileCanary(std::string name, std::string path, std::string baitContent);

    // Destructor
    ~FileCanary();

    // Override all pure virtual functions from base class (Week 10)
    void        deployCanary()         override;
    bool        checkStatus()          override;
    std::string getType()        const override;
};

#endif
