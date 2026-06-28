#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>

// ---------------------------------------------------------
// Plain file I/O helper functions — no class, no struct.
// ---------------------------------------------------------
int  readLines(const std::string& filepath, std::string outLines[], int max);
bool writeLines(const std::string& filepath, std::string lines[], int count);
bool ensureFile(const std::string& filepath);

int  splitCSV(const std::string& line, std::string outFields[], int max);
std::string joinCSV(std::string fields[], int count);

#endif // FILEMANAGER_H