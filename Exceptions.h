#pragma once
#include <stdexcept>
#include <string>

class SaveException : public std::runtime_error {
public:
    explicit SaveException(const std::string& message)
        : std::runtime_error("Save Error: " + message) {}
};

class LoadException : public std::runtime_error {
public:
    explicit LoadException(const std::string& message)
        : std::runtime_error("Load Error: " + message) {}
};