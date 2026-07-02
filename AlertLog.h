#ifndef ALERTLOG_H
#define ALERTLOG_H

#include <string>

// ============================================================
// AlertLog — Handles all alert logging (Week 4 — Static Member)
// ============================================================
// Every time a canary is triggered, we write a permanent alert
// to alerts.txt so a forensic investigator can read it later.
// The static variable keeps a count of total alerts across all instances.

class AlertLog {

private:
    std::string logFilePath;        // where alerts.txt is stored
    static int  totalAlerts;        // Week 4: shared across all AlertLog objects

public:

    // Constructor — takes the path for the log file
    AlertLog(std::string path = "alerts.txt");

    // Destructor
    ~AlertLog();

    // Writes a new alert entry to the log file
    void writeAlert(const std::string& canaryName,
                    const std::string& canaryType,
                    const std::string& canaryPath);

    // Prints everything inside alerts.txt to the console
    void showAllAlerts() const;

    // Clears the log file
    void clearLog();

    // Week 4: static function — can be called without any object
    static int getTotalAlerts();
};

#endif
