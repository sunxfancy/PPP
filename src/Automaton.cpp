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

void Automaton::init(LALRTable* ptable, AutoCallback func, Token** data) {
    table = ptable;
    function = func;
    tokens = data;
}

void Automaton::run() {
    for (int i = 1; i < table->stateSum; i++) {
        run_from(i);
    }
}

void Automaton::run_from (int state) {
    SharedStack<int> st;
    bool finished = false;
    Token* t = reader();

    if (state != -1)
        LRStack.push_back(state); // push the begin state
    int s;
    while (1) {
        if (t == 0) {
            // found a segment signal
            finished = true;
        }
        s = LRStack.back();
        printf("Stack Top: %d\n",s);
        char c = table->ACTION(s, t->type);
        int sn = table->GOTO(s, t->type);
        switch (c) {
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
            printf("Token-Type: %d\n", t->type);

            //TODO: 需要释放本层资源
            return;
        }
    }


}

void Automaton::findStack(int len) {
    if (len == max_stack) {
        // find the right way
        std::deque<int> backup(LRStack);
        run_from();
        LRStack = backup;
        return;
    }

    // find where the state from
    Token* t = reader(begin-len);
    int c = t->type;
    for (int i=0; i<=table->stateSum; ++i ) {
        int pre = table->GOTO(i, c);
        if (pre == LRStack.front()) {
            LRStack.push_front(pre);
            begin_stack.push(pre);
            findStack(len+1);
            LRStack.pop_front();
            begin_stack.pop();
        }
    }

}

void Automaton::Shift(int x, Token* t){
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
    NodeStack.push_back((void*)(t->pToken));
}



int Automaton::Reduce(int x){
    int len = table->bnf_size[x];

    if (len > LRStack.size()) {
        max_stack = len - LRStack.size();
        findStack(1);
    } else {
        std::vector<void*> args;
        for (int i = 0; i < len; ++i) {
            LRStack.pop_back();
            args.push_back(NodeStack.back());
            NodeStack.pop_back();
        }
        args.reserve(sizeof(void*));
        void* ans = function(x, args);
        NodeStack.push_back(ans);
    }
    return table->bnf_Vn[x];
}


Token* Automaton::reader() {
    return reader(now++);
}


Token* Automaton::reader(int x) {
    if (x >= begin && x < end) {
        return tokens[x];
    }
    return 0;
}
