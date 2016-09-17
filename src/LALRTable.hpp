/*
* @Author: sxf
* @Date:   2014-12-31 09:31:22
* @Last Modified by:   sxf
* @Last Modified time: 2015-12-11 19:30:36
*/

#ifndef LALR_TABLE_H
#define LALR_TABLE_H

#include <vector>
#include <cereal/types/vector.hpp>

using namespace std;

class LALRTable
{
public:
    LALRTable();
    LALRTable(const LALRTable& other);
    virtual ~LALRTable();
    LALRTable& operator=(const LALRTable& other);
    bool operator==(const LALRTable& other);

    virtual char ACTION(int, int);
    virtual int GOTO(int, int);
    virtual void printTable();

    void Save(const char* path);
    void Load(const char* path); // 需要新建一个对象,然后执行Load方法

    // 经过调整的总数，包含结束符0
    int constSum; // Vt的总数
    int stateSum; // 状态的总数
    int VSum;     // V的总数
    vector <int> bnf_size; // 每条bnf范式的长度
    vector <int> bnf_Vn; // 每条bnf范式的长度
private:
    vector < vector <int> > Goto;
    vector < vector <char> > Action;

    friend class cereal::access;

    template<class Archive>
    void serialize(Archive &ar)
    {
        ar( constSum, stateSum, VSum, CEREAL_NVP(Goto), CEREAL_NVP(Action), CEREAL_NVP(bnf_size), CEREAL_NVP(bnf_Vn) ); // serialize things by passing them to the archive
    }
};


#endif // LALR_TABLE_H
