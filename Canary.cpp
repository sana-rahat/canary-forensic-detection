#include "Canary.h"

// Constructor — saves the name and path, marks canary as not triggered
Canary::Canary(std::string name, std::string path) {
    this->name      = name;
    this->path      = path;
    this->triggered = false;
}

// Destructor
Canary::~Canary() {}

// --- Getters ---

std::string Canary::getName() const {
    return name;
}

std::string Canary::getPath() const {
    return path;
}

bool Canary::isTriggered() const {
    return triggered;
}
