#ifndef CANARY_H
#define CANARY_H

#include <string>
#include "CanaryState.h"

// ============================================================
// Canary — Abstract Base Class (Week 10)
// ============================================================
// This is the parent class for all types of canaries.
// It cannot be instantiated directly because it has pure virtual functions.
// Every specific canary type (File, Folder, Network) MUST inherit from this
// and provide their own implementation of checkStatus() and other functions.

class Canary {

protected:
    std::string  name;        // a label for this canary e.g. "PasswordTrap"
    std::string  path;        // the path being monitored e.g. "C:/trap/Passwords.txt"
    CanaryState  savedState;  // the baseline snapshot (private knowledge of the system)
    bool         triggered;   // becomes true if tampering is detected

public:

    // Constructor
    Canary(std::string name, std::string path);

    // Virtual Destructor — required when using polymorphism (Week 10)
    virtual ~Canary();

    // ---- Pure Virtual Functions (Week 10) ----
    // These MUST be implemented by every derived class.
    // Making them pure virtual (= 0) makes Canary an Abstract Class.

    virtual void deployCanary()          = 0;  // places the trap and saves baseline
    virtual bool checkStatus()           = 0;  // checks if the trap was triggered
    virtual std::string getType() const  = 0;  // returns the type name as a string

    // ---- Regular Getters ----
    std::string getName()      const;
    std::string getPath()      const;
    bool        isTriggered()  const;
};

#endif
