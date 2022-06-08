#include "quick.h"
#include <math.h>


class radix{
public:
    radix(vector<int> v_) : v(v_){
        bucket.resize(base);
        mx = 0;
        for (auto &x: v) 
            mx = x > mx ? x : mx;
    }

    vector<int> sort(){
        int pow = 1;
        while (mx >= pow) {
            for (auto &x: v) 
                bucket[x/pow % 10].push_back(x);
            auto it = v.begin();
            for (auto &b: bucket) {
                for (auto &x: b)
                    *it++ = x;
                b.clear();
            }
            pow *= 10;
        }
        return v;
    }

    vector<int> sort_print(){
        int pow = 1, cnt = 0;
        while (mx >= pow) {
            for (auto &x: v) 
                bucket[x/pow % 10].push_back(x);
            auto it = v.begin();
            for (auto &b: bucket) 
                for (auto &x: b)
                    *it++ = x;
            pow *= 10;
            print_all(cnt++);
        }
        return v;
    }

    void print_all(int cnt) {
        cout << map[cnt] + "-pass:\n";
        print_bucket();
        printf("resulting chain : ");
        print_v();
        puts("");
    }

    void print_bucket() {
        for (int i=0; i<base; ++i) {
            printf("%d | ", i);
            for (auto &x: bucket[i]) 
                printf("---> %d ", x);
            bucket[i].clear();
            puts("");
        }
    }

    void print_v(){
        for(auto vi : v){
            cout << vi << ",";
        }
        cout << endl;
    }

public:
    static const size_t base = 10;
    int mx;
    vector<string> map = {
        "First", "Second", "Third", "Fourth", "Fifth", "Sixth", "Seventh", "Eighth", "Ninth", "Tenth"
    };
    vector<int> v;
    vector<vector<int> > bucket;
};