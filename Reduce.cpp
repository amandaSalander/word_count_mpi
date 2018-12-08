//
// Created by amanda on 07/12/18.
//

#include "Reduce.h"

int Reduce::emitIntermediate(const std::string &word,const std::vector<int> &values) {
    int res=0;
    for (auto &a : values){
        res+=a;
    }
    return res;
}
