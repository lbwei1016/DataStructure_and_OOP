#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>

const int ALPH = 26;
struct HuffTree {
    HuffTree(int _ch, int _val): ch(_ch), val(_val) {
        lch = rch = nullptr;
    }
    HuffTree(int _val, HuffTree *_lch, HuffTree *_rch) {
        ch = -1;
        val = _val;
        lch = _lch;
        rch = _rch;
    }
    
    int ch;
    int val;
    HuffTree *lch, *rch;

    struct Cmp {
        bool operator()(const HuffTree *h1, const HuffTree *h2) const {
            return h1->val > h2->val;
        }
    };
};

HuffTree* build(int table[]) {
    using namespace std;
    priority_queue<HuffTree*, vector<HuffTree*>, HuffTree::Cmp> pq;
    for (int i=ALPH-1; i>=0; --i) {
        pq.push(new HuffTree(i, table[i]));
    }

    while (pq.size() > 1) {
        auto node1 = pq.top(); pq.pop();
        auto node2 = pq.top(); pq.pop();
        if (node1->val > node2->val) 
            swap(node1, node2);
        HuffTree *node = new HuffTree(node1->val + node2->val, node1, node2);
        pq.push(node);
    }
    return pq.top();
}

int encode(HuffTree *at, std::vector<std::vector<int> > &code, std::vector<int> &cur_code, int dist) {
    // a leaf
    if (at->ch != -1) {
        code[at->ch] = cur_code;
        return dist * at->val;
    }
    int sum = 0;
    if (at->lch != nullptr) {
        cur_code.push_back(0);
        sum += encode(at->lch, code, cur_code, dist+1);
        cur_code.pop_back();
    }
    if (at->rch != nullptr) {
        cur_code.push_back(1);
        sum += encode(at->rch, code, cur_code, dist+1);
        cur_code.pop_back();
    }
    return sum;
}

int main(int argc, const char* argv[]){
    int table[ALPH] = {11, 5, 2, 3, 12, 3, 5, 6, 8, 1, 2, 4, 8, 7, 2, 2, 1, 9, 6, 2, 4, 1, 10, 10, 1, 1};

    std::vector<std::string> all_args;
    all_args.assign(argv, argv + argc); // convert command line argument to string 

    std::cout << "DSOO-Program2-Demo: ";
    for(auto a : all_args){
        std::cout << a << " ";
    }
    std::cout << "\n";
    
	

    
    if(argc != 2){
        std::cout << "part1\n";
    }else{
        std::string filename = argv[1];
        std::cout << "read file from : " << "test/" << filename << std::endl;
    }
    
    for (int i=0; i<ALPH; ++i) {
        printf("%c = %2d |", i+'A', table[i]);
        if (i % 10 == 9 || i == ALPH - 1) putchar('\n');
        else putchar(' ');
    }

    std::vector<std::vector<int> > code(26, std::vector<int>());
    std::vector<int> tmp;
    HuffTree *root = build(table);
    int WEPL = encode(root, code, tmp, 0); // weighted external path length

    for (int i=0; i<ALPH; ++i) {
        printf("%c : ", i+'A');
        for (auto x: code[i]) {
            printf("%d", x);
        }
        puts("");
    }

    printf("\nWEPL : %d\n", WEPL);
    return 0;
}