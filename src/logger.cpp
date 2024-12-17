#include "logger.h"
#include <iostream>
#include <iomanip>
#include <ctime>

Logger::Logger(const std::string& logFilePath) : isLoggingEnabled(true) {
    // Open the specified log file in append mode
    logFile.open(logFilePath, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Error: Unable to open " << logFilePath << " for logging.\n";
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::logKey(const std::string& key) {
    if (!isLoggingEnabled || !logFile.is_open()) {
        return;
    }

    std::lock_guard<std::mutex> guard(logMutex);  // Ensures thread safety

    // Append a timestamp with the logged key
    std::time_t now = std::time(nullptr);
    logFile << "[" << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S") << "] ";
    logFile << key << "\n";

    // Ensure data is flushed to the file immediately
    logFile.flush();
}

void Logger::toggleLogging(bool enable) {
    isLoggingEnabled = enable;  // Toggle logging on/off
}
