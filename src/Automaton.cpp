#include "Automaton.hpp"
#include "LALRTable.hpp"
#include "ParallelManager.hpp"
Automaton::Automaton (ParallelWorker* pm) {
    this->pm = pm;
}

Automaton::Automaton (const Automaton& other) {
    table = other.table;
    function = other.function;
    LRStack = other.LRStack;
    NodeStack = other.NodeStack;
}

Automaton::~Automaton () {

}

void Automaton::init(LALRTable* ptable, AutoCallback func, const Token* data) {
    table = ptable;
    function = func;
    tokens = data;
}

void Automaton::run() {
    if (begin != 0)
        for (int i = 1; i < table->stateSum; i++) {  // i一定不能从0开始，否则就成为了第一个根BNF
            now = begin;
            LRStack.clear();
            run_from(i);
        }
    else {
        now = begin;
        LRStack.clear();
        run_from(0);
    }
}

void Automaton::run_from (int state) {
    bool finished = false;
    const Token* t = reader();

    if (state != -1)
        LRStack.push_back(state); // push the begin state
    int s;
    while (1) {
        cout << "------------------------" << endl;
        printf("Token: %s %d\n", t->pToken, t->type);
        if (now == end) {
            // found a segment signal
            finished = true;
        }
        s = LRStack.back();
        if (s < 0) {
            printf("LRCore error\n");
            return;
        }
        printf("Stack Top: %d\n",s);
        char c = table->ACTION(s, t->type);
        int sn = table->GOTO(s, t->type);
        printf("type: %d, action: %c, goto: %d\n", t->type, c, sn);
        switch (c) {
            case 'a': {
                printf("Accept!\n");
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
                return;
            }
        }
    }


}

void Automaton::findStack(int len, int shift_size) {
    if (len == max_stack) {
        // find the right way
        std::deque<int> backup(LRStack);
        int now_back = now;
        run_from();
        LRStack = backup;
        now = now_back;
        return;
    }

    // find where the state from
    const Token* t = reader(begin-shift_size);
    int c = t->type;
    for (int i=0; i<=table->stateSum; ++i ) {
        int pre = table->GOTO(i, c);
        if (pre == LRStack.front()) {
            LRStack.push_front(pre);
            begin_stack.push_front(pre);
            findStack(len+1, shift_size+1);
            LRStack.pop_front();
            begin_stack.pop_front();
        }

        // TODO: VMap need check, for Vn translate
        for ( int j = table->constSum; j <= table->VSum; ++j ) {
            int pre = table->GOTO(i, j);
            if (pre == LRStack.front()) {
                LRStack.push_front(pre);
                begin_stack.push_front(pre);
                findStack(len+1, shift_size);
                LRStack.pop_front();
                begin_stack.pop_front();
            }
        }
    }
}

void Automaton::Shift(int x, const Token* t){
    LRStack.push_back(x);
    NodeStack.push_back((void*)(t->pToken));

    // for debug
    auto& fout = cout;
    fout << "Stack: ";
    for (auto p: LRStack)
    {
        fout << p << ' ';
    }
    fout << endl << "Shift: " << x << endl;
}



int Automaton::Reduce(int x){
    int len = table->bnf_size[x];
    if (len >= LRStack.size()) {
        max_stack = (int) (len - LRStack.size() + 1);
        findStack(1, 1);
    }
    std::vector<void*> args;
    for (int i = 0; i < len; ++i) {
        LRStack.pop_back();
        args.push_back(NodeStack.back());
        NodeStack.pop_back();
    }
    args.reserve(sizeof(void*));
    void* ans = function(x, args);
    NodeStack.push_back(ans);
    int Vn = table->bnf_Vn[x];
    int s = LRStack.back();
    LRStack.push_back(table->GOTO(s, Vn));

    // for debug
    auto& fout = cout;
    fout << "Stack: ";
    for (auto p: LRStack)
    {
        fout << p << ' ';
    }
    fout << endl << "Reduce: " << x << endl;

    return table->GOTO(s, Vn);
}


const Token* Automaton::reader() {
    return reader(now++);
}


const Token* Automaton::reader(int x) {
    if (x >= 0 && x <= end)
        return &tokens[x];
    return 0;
}
