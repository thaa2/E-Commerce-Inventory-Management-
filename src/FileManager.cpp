#include "../include/FileManager.h"
#include <fstream>
#include <sstream>

using namespace std;

int readLines(const string& filepath, string outLines[], int max) {
    ifstream file(filepath);
    if (!file.is_open()) {
        return 0;
    }

    int count = 0;
    string line;
    while (count < max && getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        outLines[count] = line;
        count++;
    }

    file.close();
    return count;
}

bool writeLines(const string& filepath, string lines[], int count) {
    ofstream file(filepath, ios::trunc);
    if (!file.is_open()) {
        return false;
    }

    for (int i = 0; i < count; i++) {
        file << lines[i] << "\n";
    }

    file.close();
    return true;
}

bool ensureFile(const string& filepath) {
    ifstream check(filepath);
    if (check.good()) {
        check.close();
        return true;
    }
    check.close();

    ofstream create(filepath);
    if (!create.is_open()) {
        return false;
    }
    create.close();
    return true;
}

int splitCSV(const string& line, string outFields[], int max) {
    int count = 0;
    string current = "";
    bool inQuotes = false;

    for (size_t i = 0; i < line.size(); i++) {
        char c = line[i];

        if (c == '"') {
            inQuotes = !inQuotes;
            continue;
        }

        if (c == ',' && !inQuotes) {
            if (count < max) {
                outFields[count] = current;
                count++;
            }
            current = "";
        } else {
            current += c;
        }
    }

    if (count < max) {
        outFields[count] = current;
        count++;
    }

    return count;
}

string joinCSV(string fields[], int count) {
    ostringstream oss;
    for (int i = 0; i < count; i++) {
        string field = fields[i];

        if (field.find(',') != string::npos) {
            field = "\"" + field + "\"";
        }

        oss << field;
        if (i < count - 1) {
            oss << ",";
        }
    }
    return oss.str();
}