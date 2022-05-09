#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <fstream>
#include <cstring>

const int ALPH = 26+26;
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
            return h1->val == h2->val ? h1->ch < h2->ch : h1->val > h2->val;
        }
    };
};

HuffTree* build(int table[]) {
    using namespace std;
    priority_queue<HuffTree*, vector<HuffTree*>, HuffTree::Cmp> pq;
    for (int i=ALPH-1; i>=0; --i) {
        if (!table[i]) continue;
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

bool decode(HuffTree *root, std::string &code) {
    HuffTree *at = root;
    int id = 0;
    while (id < code.size()) {
        if (at == nullptr) return false;
       
        if (code[id] == '0') {
            at = at->lch;
            ++id;
        }
        else {
            at = at->rch;
            ++id;
        }
        // leaf
        if (at->ch >= 0) {
            char c;
            if (at->ch > (ALPH-1)/2) c = 'a' + at->ch - ALPH/2;
            else c = 'A' + at->ch;
            printf("%c", c);
            at = root;
        }
    }
    return at == root;
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
    


    std::string text;
    if(argc != 2){
        std::cout << "part1\n";
    }else{
        std::string filename = argv[1];
        std::cout << "read file from : " << "test/" << filename << std::endl;
        
        memset(table, 0, sizeof(table));
        std::ifstream iff(filename, std::ios::in);

        iff >> text;
        std::cout << "characters : " + text + '\n';
        for (auto x: text) {
            int d = x - 'a';
            // uppercase
            if (d < 0) {
                d = x - 'A';
                ++table[d];
            }
            else ++table[d+ALPH/2];
        }

        iff >> text;
        iff.close();
    }

    // print characters to be encoded
    int cnt = 0;
    for (int i=0; i<ALPH/2; ++i) {
        if (!table[i]) continue;

        ++cnt;
        printf("%c = %2d |", i+'A', table[i]);
        if (cnt % 10 == 0) putchar('\n');
        else putchar(' ');
    }
    for (int i=ALPH/2; i<ALPH; ++i) {
        if (!table[i]) continue;

        ++cnt;
        printf("%c = %2d |", i-ALPH/2+'a', table[i]);
        if (cnt % 10 == 0) putchar('\n');
        else putchar(' ');
    }
    puts("");
    if (cnt % 10 != 0) puts("");

    // build Huffman tree and encode
    std::vector<std::vector<int> > code(ALPH, std::vector<int>());
    std::vector<int> tmp;
    HuffTree *root = build(table);
    int WEPL = encode(root, code, tmp, 0); // weighted external path length

    for (int i=0; i<ALPH/2; ++i) {
        if (!table[i]) continue;
        printf("%c : ", i+'A');
        for (auto x: code[i]) {
            printf("%d", x);
        }
        puts("");
    }

    for (int i=ALPH/2; i<ALPH; ++i) {
        if (!table[i]) continue;
        printf("%c : ", i-ALPH/2+'a');
        for (auto x: code[i]) {
            printf("%d", x);
        }
        puts("");
    }

    if (text.size()) {
        std::cout << "\nHuffman code : "+text+'\n';
        std::cout << "decode : ";
    }

    bool status = decode(root, text);
    if (!status) printf(" -wrong code-\n");
    printf("\nWEPL : %d\n", WEPL);
    return 0;
}