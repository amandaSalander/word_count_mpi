//
// Created by amanda on 07/12/18.
//

#ifndef HELLO_MPI_COMBINER_H
#define HELLO_MPI_COMBINER_H


#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <regex>
#include <algorithm>

class Combiner {
public:
    std::map<std::string, std::vector<int>*>* combineResult(const std::string &filename);
};


#endif //HELLO_MPI_COMBINER_H
