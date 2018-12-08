//
// Created by amanda on 06/12/18.
//


#include <sstream>
#include <iterator>
#include "Map.h"
/** This function hash generate a hash from a string**/
unsigned long long  hash( std::string const& s )
{
    unsigned long long results = 1; //  anything but 0 is probably OK.
    for ( auto current = s.begin(); current != s.end(); ++ current ) {
        results = 127 * results + static_cast<unsigned char>( *current );
    }
    return results;
}

Map::Map() {
    lines= nullptr;
}

std::vector<std::string> *Map::getLines() const {
    return lines;
}

void Map::setLines(std::vector<std::string> *lines) {
    Map::lines = lines;
}

void Map::emitIntermediate(std::string w, size_t count) {

    std::ofstream file("intermidate_map/"+std::to_string(hash(w)),std::ios::out | std::ios::app);

    file << w << " " << count <<std::endl;

    file.close();
}

 std::vector<std::string>*Map::generateKeyValueList(const std::string &line) {
    std::stringstream ss(line);
    std::string item;
    char delimeter=' ';
    auto *splittedStrings= new std::vector<std::string>();
    while (std::getline(ss, item, delimeter))
    {
        splittedStrings->push_back(item);
//        std::cout << item << std::endl;
    }
    return splittedStrings;
}
