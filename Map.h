//
// Created by amanda on 06/12/18.
//

#ifndef HELLO_MPI_MAP_H
#define HELLO_MPI_MAP_H


#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>


class Map {
private:
    std::vector<std::string> *lines;
public:
    Map();

    std::vector<std::string> *getLines() const;

    void setLines(std::vector<std::string> *lines);

    void emitIntermediate(std::string w,size_t count=1);

    std::vector<std::string> *generateKeyValueList(const std::string &line);
};


#endif //HELLO_MPI_MAP_H
