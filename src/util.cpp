#include "util.h"
#include <fstream>
#include <iostream>

auto read_file(const std::string& filename) -> const char* {
    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(1);
    }

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* buffer = new char[size + 1];  // +1 for the null terminator
    file.read(buffer, size);
    buffer[size] = '\0';

    if (!file) {
        std::cerr << "Error reading file: " << filename << std::endl;
        delete[] buffer;
        exit(1);
    }

    return buffer;
}

