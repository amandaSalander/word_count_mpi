//
// Created by amanda on 07/12/18.
//

#include "Combiner.h"



std::map<std::string, std::vector<int>*>* Combiner::combineResult(const std::string &filename) {
    std::ifstream in("intermidate_map/"+filename);
    std::string line;


    /** get the word in the first line **/
    std::getline(in,line);

    std::string word=line.substr(0,line.find(' '));

    if (word.find('.')) {
        word= word.substr(0,word.find('.'));
    }
    if (word.find(',')) {
        word= word.substr(0,word.find(','));
    }
    if (word.find(':')) {
        word= word.substr(0,word.find(':'));
    }

    /** convert word to lower case **/

    std::transform(word.begin(), word.end(), word.begin(), ::tolower);



    auto *v = new std::vector<int>();
    v->emplace_back(
            std::stoi(line.substr(line.find(' ')+1))
            );
    while (std::getline(in,line)){
        v->emplace_back(
                std::stoi(line.substr(line.find(' ')+1))
        );
    }

    auto *p=new std::map<std::string, std::vector<int>*>();
    p->operator[](word)=v;

    in.close();
    return p;

}