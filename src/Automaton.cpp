#include "Automaton.hpp"
#include "LALRTable.hpp"
#include "ParallelManager.hpp"
Automaton::Automaton (ParallelManager* pm) {
    this->pm = pm;
}

Automaton::Automaton (const Automaton& other) {
    table = other.table;
    function = other.function;
    lex = other.lex;
    LRStack = other.LRStack;
    NodeIntStack = other.NodeIntStack;
    NodeStack = other.NodeStack;
}

Automaton::~Automaton () {

}

void Automaton::init(LALRTable* ptable, AutoCallback func) {
    table = ptable;
    function = func;
}

void Automaton::run() {
    for (int i = 1; i < table->stateSum; i++) {
        run_from(i);
    }
}

void Automaton::run_from (int state) {
    SharedStack<int> st;
    bool finished = false;
    void* pdata = NULL;
    int t = reader(&pdata);

    LRStack.push_back(state); // push the begin state
    int s;
    while (1) {
        if (t == -4) {
            // found a segment signal
            t = reader(&pdata);
            finished = true;
        }
        s = LRStack.back();
        printf("Stack Top: %d\n",s);
        char c = table->ACTION(s, t);
        int sn = table->GOTO(s, t);
        switch (c) {
        case 's': {
            if (finished) {
                pm->finish(this);
                return;
            }
            Shift(sn, t, pdata);
            pdata = NULL;
            t = reader(&pdata);
            break;
        }
        case 'r': {
            int Vn = Reduce(sn);
            s = LRStack.back();
            printf("Stack Top: %d, Vn:%d\n", s, Vn);
            break;
        }
        case 'a':
            printf("Accept!\n");
            return;
        default:
            printf("LRCore error\n");
            printf("错误的Action动作：%c\n", c);
            printf("目前的状态：%d\n", s);
            printf("Token-Type: %d\n", t);

            //TODO: 需要释放本层资源
            return;
        }
    }


}

void Automaton::findStack(int len) {
    if (len == 0) {
        // find the right way

        return;
    }


}

void Automaton::Shift(int x, int t, void* ptr){
    // for debug
    auto& fout = cout;
    fout << "------------------------" << endl;
    fout << "Stack: ";
    for (auto p: LRStack)
    {
        fout << p << ' ';
    }
    fout << "Shift: " << x << endl;
    LRStack.push_back(x);
    NodeIntStack.push_back(t);
    NodeStack.push_back(ptr);
}



int Automaton::Reduce(int x){
    int len = table->bnf_size[x];

    if (len > LRStack.size()) {
        findStack(len - LRStack.size());
    } else {
        std::vector<void*> args;
        for (int i = 0; i < len; ++i) {
            LRStack.pop_back();
            NodeIntStack.pop_back();
            args.push_back(NodeStack.back());
            NodeStack.pop_back();
        }
        args.reserve(sizeof(void*));
        void* ans = function(x, args);
        NodeStack.push_back(ans);
    }
    return table->bnf_Vn[x];
}
