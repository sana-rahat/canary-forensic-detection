#include "CanaryState.h"

// Default constructor — initializes everything to zero/empty
CanaryState::CanaryState() {
    fileSize     = 0;
    checksum     = "";
    lastModified = 0;
}

// Parameterized constructor — sets all values directly
CanaryState::CanaryState(long long size, std::string checksum, std::time_t modified) {
    this->fileSize     = size;
    this->checksum     = checksum;
    this->lastModified = modified;
}

// Two states are DIFFERENT if ANY of the three values changed
bool CanaryState::operator!=(const CanaryState& other) const {
    return (fileSize     != other.fileSize)     ||
           (checksum     != other.checksum)     ||
           (lastModified != other.lastModified);
}

// Two states are the SAME if none of the three values changed
bool CanaryState::operator==(const CanaryState& other) const {
    return !(*this != other);
}
