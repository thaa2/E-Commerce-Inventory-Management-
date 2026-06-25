#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>

// ---------------------------------------------------------
// Plain file I/O helper functions — no class, no struct.
// ---------------------------------------------------------
static int  readLines(const std::string& filepath, std::string outLines[], int max);
static bool writeLines(const std::string& filepath, std::string lines[], int count);
static bool ensureFile(const std::string& filepath);

static int  splitCSV(const std::string& line, std::string outFields[], int max);
static std::string joinCSV(std::string fields[], int count);

#endif // FILEMANAGER_H