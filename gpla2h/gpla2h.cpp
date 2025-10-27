#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <direct.h>
#include <vector>

static uint64_t header[] = {
    0x20202020414C5047,               // "GPLA    "
    2,                                // 2
    0,                                // 0
    0,                                // 0
    0x20202020454C4946,               // "FILE    "
};

#define G_FRAME    0x202020454D415246 // "FRAME   "
#define G_OBJECTS  0x20535443454A424F // "OBJECTS "
#define G_OBJECT   0x20205443454A424F // "OBJECT  "
#define G_MATRIX   0x202058495254414D // "MATRIX  "
#define G_DONE     0x20202020454E4F44 // "DONE    "
#define G_STROKES  0x2053454B4F525453 // "STROKES "
#define G_STROKE   0x2020454B4F525453 // "STROKE  "
#define G_VTX_CT   0x7443786574726576 // "vertexCt"
#define G_VERTICES 0x5345434954524556 // "VERTICES"

#define ELEMENTS_PER_LINE 8

struct Vertex {
    float x;
    float y;
    float z;
    bool operator==(const Vertex& rhs) const
    {
        return (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
    }
};

int checkVertex(double* where, std::vector<Vertex>& verticesList) {
    Vertex v;
    v.x = where[0];
    v.y = where[1];
    v.z = where[2];

    int found;
    for (found = 0; found < verticesList.size(); found++) {
        if (verticesList[found] == v) break;
    }
    if (found == verticesList.size()) verticesList.push_back(v);
    return found;
}

int main()
{
    std::vector<Vertex> verticesList;
    std::vector<int> indicesList;
    std::vector<int> lengthsList;

    const size_t size = 1024;
    char buffer[size];
    if (_getcwd(buffer, size) != NULL) {
        std::cout << "GPLA2H - starting in directory " << buffer << std::endl;
    }
    else {
        std::cerr << "Error getting current working directory" << std::endl;
        return 1;
    }

    std::string infname;
    std::cout << "GPLA2H - [Enter input filename] ";
    std::cin >> infname;

    std::string outfname;
    std::cout << "GPLA2H - [Enter output filename] ";
    std::cin >> outfname;

    std::string name, nameUpper;
    std::cout << "GPLA2H - [Enter the name of the variable] ";
    std::cin >> name;
    nameUpper = name;
    std::transform(nameUpper.begin(), nameUpper.end(), nameUpper.begin(), ::toupper);

    std::cout << "GPLA2H - opening input file..." << std::endl;
    std::ifstream inFile;
    inFile.open(infname, std::ifstream::binary);
    std::streampos fSize = inFile.tellg();
    inFile.seekg(0, std::ios::end);
    fSize = inFile.tellg() - fSize;
    inFile.seekg(0, std::ios::beg);

    std::cout << "GPLA2H - reading input file..." << std::endl;
    uint64_t* dat = new uint64_t[1 + fSize / 8];
    inFile.read(reinterpret_cast<char*>(dat), fSize);
    inFile.close();

    int pos = fSize / 8;
    int tempPos = 0;
    for (pos = 0; pos < 5; pos++) {
        if (dat[pos] != header[pos]) {
            std::cerr << "GPLA2H - Invalid header at position " << pos * 8 << "! Exiting." << std::endl;
            delete[] dat;
            return 2;
        }
    }
    tempPos = pos;

    std::cout << "GPLA2H - header is valid! extracting strokes..." << std::endl;

    while (dat[pos++] != G_DONE) {
        pos++;
    }
    if (dat[pos] != G_FRAME) {
        std::cerr << "GPLA2H - Frame not found, expected at position " << pos * 8 << ". Exiting." << std::endl;
        delete[] dat;
        return 3;
    }
    pos++;

    while (dat[pos++] != G_OBJECTS) {
        pos++;
    }
    
    bool done1 = false;
    bool done2 = false;
    int strokeLen = 0;
    while (!done1) {
        if (dat[pos] == G_DONE) {
            done1 = true;
            pos++;
            break;
        } else if (dat[pos] != G_OBJECT) {
            std::cerr << "GPLA2H - Object not found, expected at position " << pos * 8 << ". Exiting." << std::endl;
            delete[] dat;
            return 4;
        }
        pos++;
        if (dat[pos] == G_MATRIX) {
            pos += 18;
        }
        if (dat[pos] == G_STROKES) {
            pos++;
            done2 = false;
            while (!done2) {
                if (dat[pos] == G_DONE) {
                    done2 = true;
                    pos++;
                    break;
                }
                else if (dat[pos] != G_STROKE) {
                    std::cerr << "GPLA2H - Stroke not found, expected at position " << pos * 8 << ". Exiting." << std::endl;
                    delete[] dat;
                    return 5;
                }
                pos++;

                while (dat[pos++] != G_VERTICES) {
                    if (dat[pos - 1] == G_VTX_CT) {
                        strokeLen = dat[pos];
                        lengthsList.push_back(strokeLen);
                    }
                    pos++;
                }

                for (int i = 0; i < strokeLen; i++) {
                    indicesList.push_back(checkVertex(reinterpret_cast<double*>(dat + pos), verticesList));
                    pos += 3;
                }
                if (dat[pos] != G_DONE) {
                    std::cerr << "GPLA2H - Vertex list didn't end at position " << pos * 8 << ". Exiting." << std::endl;
                    delete[] dat;
                    return 6;
                }
                pos++;

                if (dat[pos] != G_DONE) {
                    std::cerr << "GPLA2H - Stroke didn't end at position " << pos * 8 << ". Exiting." << std::endl;
                    delete[] dat;
                    return 6;
                }
                pos++;
            }
        }
    }

    std::cout << "GPLA2H - opening output file..." << std::endl;
    std::ofstream outFile;
    outFile.open(outfname);

    outFile << "#pragma once\n";
    outFile << "// AUTO-GENERATED BY GPLA2H\n\n";

    outFile << "#define " << nameUpper << "_STROKES " << std::to_string(lengthsList.size()) << "\n";

    outFile << "static float " << name << "Vertices[] = {\n";
    for (int i = 0; i < verticesList.size(); i++) {
        outFile << "    "
            << std::to_string(verticesList[i].x) << ", "
            << std::to_string(verticesList[i].y) << ", "
            << std::to_string(verticesList[i].z) << ",\n";
    }
    outFile << "};\n\n";

    outFile << "static int " << name << "Indices[] = {";
    for (int i = 0; i < indicesList.size(); i++) {
        if (i % 8 == 0) outFile << "\n   ";
        outFile << " " << std::to_string(indicesList[i]) << ",";
    }
    outFile << "\n};\n\n";

    outFile << "static int " << name << "Strokes[] = {";
    for (int i = 0; i < lengthsList.size(); i++) {
        if (i % 8 == 0) outFile << "\n   ";
        outFile << " " << std::to_string(lengthsList[i]) << ",";
    }
    outFile << "\n};\n\n";

    outFile.close();
    delete[] dat;
    return 0;
}
