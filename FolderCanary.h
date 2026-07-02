#ifndef FOLDERCANARY_H
#define FOLDERCANARY_H

#include "Canary.h"
#include <vector>

// ============================================================
// FolderCanary — Derived Class (Week 9, 10)
// ============================================================
// This canary monitors a folder/directory.
// It remembers how many files were originally inside.
// If a file is added or removed — it raises an alert.

class FolderCanary : public Canary {

private:
    int originalFileCount;      // how many files were in the folder at deployment

    // Helper: counts how many files are currently inside the folder
    int countFilesInFolder();

public:

    // Constructor
    FolderCanary(std::string name, std::string path);

    // Destructor
    ~FolderCanary();

    // Override pure virtual functions (Week 10)
    void        deployCanary()          override;
    bool        checkStatus()           override;
    std::string getType()         const override;
};

#endif
