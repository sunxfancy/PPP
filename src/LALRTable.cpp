/*
* @Author: sxf
* @Date:   2015-01-03 13:21:45
* @Last Modified by:   sxf
* @Last Modified time: 2015-12-22 14:13:44
*/

#include "LALRTable.hpp"
#include <iostream>
#include <fstream>
#include <cereal/archives/portable_binary.hpp>


LALRTable::LALRTable() {

}

LALRTable::LALRTable(const LALRTable& other){

}

LALRTable::~LALRTable(){

}

LALRTable& LALRTable::operator=(const LALRTable& other){
	return *this;
}

bool LALRTable::operator==(const LALRTable& other){
	return true;
}



char LALRTable::ACTION(int state, int id){
    if (state < 0 || state >= stateSum) return 0;
    if (id < 0 || id >= constSum) return 0;
    return Action[state][id];
}

int LALRTable::GOTO(int state, int id){
    if (state < 0 || state >= stateSum) return 0;
    if (id < 0 || id >= VSum) return 0;
    return Goto[state][id];
}

void LALRTable::printTable() {
    auto& fout = std::cout;
    fout << "=========== LALR TABLE ==========" << endl;
    for (int s = 0; s < VSum; ++s) {
        fout << "\t" << s;
    }
    fout << endl;
    for (int s = 0; s < stateSum; ++s) {
        fout << "I" << s << ":";
        for (int i = 0; i < VSum; ++i) {
            fout << "\t";
            if (ACTION(s,i)=='a') {fout << "acc"; continue;  }
            if (ACTION(s,i))
                fout << ACTION(s,i);
            fout << GOTO(s,i);
        }
        fout << endl;
    }
}


void LALRTable::Save(const char* path) {
    std::ofstream os(path, std::ios::out | std::ios::binary | std::ios::trunc );
    cereal::PortableBinaryOutputArchive oarchive(os);
    oarchive( cereal::make_nvp("myData", *this) );
}

void LALRTable::Load(const char* path) {
    std::ifstream is(path, std::ios::in | std::ios::binary);
    cereal::PortableBinaryInputArchive iarchive(is);
    iarchive( cereal::make_nvp("myData", *this) );
}
