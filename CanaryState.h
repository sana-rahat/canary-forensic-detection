#ifndef CANARYSTATE_H
#define CANARYSTATE_H

#include <string>
#include <ctime>

// CanaryState stores a "snapshot" of a file at a specific moment.
// We save this snapshot when we first deploy the canary.
// Later, we take a new snapshot and compare — if they differ, someone tampered with the file.

struct CanaryState {

    long long   fileSize;       // size of the file in bytes
    std::string checksum;       // simple character sum to detect content changes
    std::time_t lastModified;   // last time the file was modified

    // Default constructor — empty state
    CanaryState();

    // Parameterized constructor — create a state with known values
    CanaryState(long long size, std::string checksum, std::time_t modified);

    // Operator Overloading (Week 6)
    // Returns true if the two states are DIFFERENT (i.e. file was changed)
    bool operator!=(const CanaryState& other) const;

    // Returns true if the two states are exactly the SAME (file is safe)
    bool operator==(const CanaryState& other) const;
};

#endif
