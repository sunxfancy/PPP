/**
* @Author: Sun Xiaofan <sxf>
* @Date:   2016-10-29
* @Email:  sunxfancy@gmail.com
* @Last modified by:   sxf
* @Last modified time: 2016-11-17
* @License: MIT License
*/



#include "Automaton.hpp"
#include "LALRTable.hpp"
#include "ParallelManager.hpp"
Automaton::Automaton (ParallelWorker* pm) {
    this->pm = pm;
}

Automaton::Automaton (const Automaton& other) {
    begin_stack = other.begin_stack;
    LRStack = other.LRStack;
    begin = other.begin;
    end = other.end;
    now = other.now;
    SymbolStack = other.SymbolStack;
    begin_symbol = other.begin_symbol;
//    NodeStack = other.NodeStack;
}

Automaton::~Automaton () {

}

void Automaton::init(LALRTable* ptable, AutoCallback func, const std::vector<Token>& data) {
    table = ptable;
    function = func;
    tokens = &data;
}

void Automaton::run() {
    if (begin != 0)
        for (int i = 1; i < table->stateSum; i++) {  // i一定不能从0开始，否则就成为了第一个根BNF
            now = begin;
            LRStack.clear();
            begin_stack.clear();
            SymbolStack.clear();
            begin_symbol.clear();
            run_from(i);
        }
    else {
        now = begin;
        begin_stack.clear();
        LRStack.clear();
        SymbolStack.clear();
        begin_symbol.clear();
        run_from(0);
    }
}

void Automaton::run_from (int state) {
    bool finished = false;
    const Token* t = reader();

    if (state != -1) {
        // first begin
        LRStack.push_back(state); // push the begin state
        SymbolStack.push_back(0);
        begin_stack.push_front(state);
        begin_symbol.push_front(0);

        if (table->ACTION(state, t->type) == 'r'
            && table->bnf_size[table->GOTO(state, t->type)] >= LRStack.size()
                ) return;// for the first action can not be Reduce
    }
    int s = LRStack.back();

    while (1) {
        if (now > end) {
            // found a segment signal
            finished = true;
        }
        s = LRStack.back();
        if (s < 0 || t== nullptr || t->type == -1) {
            printf("LRCore error\n");
            return;
        }
        char c = table->ACTION(s, t->type);
        int sn = table->GOTO(s, t->type);
        // if (begin != 0) {
            // cout << "------------------------" << endl;
            // printf("now: %d, begin: %d, end: %d\n", now, begin, end);
            // printf("Token: %s %d\n", t->pToken, t->type);
            // printf("Stack Top: %d\n", s);
            // printf("type: %d, action: %c, goto: %d\n", t->type, c, sn);
        // }
        switch (c) {
            case 'a': {
                // printf("Accept!\n");
                pm->finish(this);
                return;
            }
            case 's': {
                if (finished) {
                    pm->finish(this);
                    return;
                }
                Shift(sn, t);
                t = reader();
                break;
            }
            case 'r': {
                int Vn = Reduce(sn);
                if (Vn != -1) break;
            }
            default: {
                /*
                string pointer = t->debug_line;
                for (auto p = pointer.begin(); p != pointer.end(); ++p) {
                    if ((p - pointer.begin()) == t->col_num) {
                        *p = '^';
                        continue;
                    }
                    if (*p != ' ' && *p != '\t') {
                        *p = '~';
                        continue;
                    }
                }
                printf("%s\n", t->debug_line);
                printf("%s\n", pointer.c_str());
                printf("LRCore error\n");
                printf("错误的Action动作：%c\n", c);
                printf("目前的状态：%d\n", s);
                printf("Token-Type: %d\n", t->type);
                printf("Token: %s\n", t->pToken);
                printf("line: %d, %d\n", t->row_num, t->col_num);
                //TODO: 需要释放本层资源
                */
                return;
            }
        }
    }


}

void Automaton::findStack(int x, int Vn) {
    // find the right way
    std::deque<int> backup = this->LRStack;
    std::deque<int> begin_backup = this->begin_stack;
    std::deque<int> sym_backup = this->SymbolStack;
    std::deque<int> begin_sym_backup = this->begin_symbol;
    int now_back = this->now;
    int next;

    auto& bnf_from = table->bnf_from[x];
    auto& bnf = table->bnfs[x];
    vector<int> data;
    vector<int> symbol;
    for (int begin_state : bnf_from) {
        int s = begin_state;
        int i;
        data.push_back(s);
        symbol.push_back(0);
        for (i = 0; i < bnf.size(); ++i) {
            s = table->GOTO(s, bnf[i]);
            if (s == begin_stack.front()) break;
            data.push_back(s);
            symbol.push_back(bnf[i]);
        }
        if (s != begin_stack.front()) goto RESTORE; // 必须要转移到当前状态

        begin_symbol.front() = bnf[i];
        while (!data.empty()) {
            this->begin_stack.push_front(data.back());
            this->begin_symbol.push_front(symbol.back());
            data.pop_back();
            symbol.pop_back();
        }
        next = table->GOTO(begin_state, Vn);
        if (next == -1) goto RESTORE; // next必须正确转移

        LRStack.clear();
        LRStack.push_back(begin_state);
        LRStack.push_back(next);
        SymbolStack.clear();
        SymbolStack.push_back(0);
        SymbolStack.push_back(Vn);

        // run
        now--;
        run_from();

RESTORE:// restore
        this->begin_stack = begin_backup;
        this->LRStack = backup;
        this->now = now_back;
        this->SymbolStack = sym_backup;
        this->begin_symbol = begin_sym_backup;
    }
}

void Automaton::Shift(int x, const Token* t){
    LRStack.push_back(x);
    SymbolStack.push_back(t->type);
//    NodeStack.push_back((void*)(t->pToken));

    // for debug
//     auto& fout = cout;
//     fout << "Stack: ";
//     for (auto p: LRStack)
//     {
//         fout << p << ' ';
//     }
//     fout << endl << "Shift: " << x << endl;
}

int Automaton::Reduce(int size, int Vn, int x) {
//    std::vector<void*> args;
    for (int i = 0; i < size; ++i) {
        LRStack.pop_back();
        SymbolStack.pop_back();
//        args.push_back(NodeStack.back());
//        NodeStack.pop_back();
    }
//    args.reserve(sizeof(void*));
//    void* ans = function(x, args);
//    NodeStack.push_back(ans);

    int s = LRStack.back();
    LRStack.push_back(table->GOTO(s, Vn));
    SymbolStack.push_back(Vn);

    // for debug
//     auto& fout = cout;
//     fout << "Stack: ";
//     for (auto p: LRStack)
//     {
//         fout << p << ' ';
//     }
//     fout << endl << "Reduce: " << x << endl;

    return table->GOTO(s, Vn);
}

int Automaton::Reduce(int x){
    int len = table->bnf_size[x];
    int Vn = table->bnf_Vn[x];
    if (len >= LRStack.size() ) {
        findStack(x, Vn);
        return -1;
    }

    return Reduce(len, Vn, x);
}


const Token* Automaton::reader() {
    return reader(now++);
}


const Token* Automaton::reader(int x) {
    if (x >= 0 && x <= end)
        return tokens->data()+x;
    return nullptr;
}
