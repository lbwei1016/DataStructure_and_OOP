#include "insertion.h"

class quick{
public:
    quick(vector<int> v_): v(v_){}

    vector<int> sort(){
        quick_sort_partation(0, v.size()-1);
        return v;
    }

    vector<int> sort_print(){
        quick_sort_partation_print(0, v.size()-1);
        return v;
    }

    void quick_sort_partation_print(int front, int rear){
        if (rear - front < 1) return;
        int pivot = v[rear], i = front-1, j = front;
        
        while (j <= rear) {
            if (v[j] < pivot) 
                swap(v[++i], v[j]);
            ++j;
        }
        swap(v[++i], v[rear]);

        printf("%d-%d : \n", front, rear);
        print_v();
        quick_sort_partation_print(front, i-1); quick_sort_partation_print(i+1, rear);
        return;
    }

    void quick_sort_partation(int front, int rear){
        if (rear - front < 1) return;
        int pivot = v[rear], i = front-1, j = front;
        
        while (j <= rear) {
            if (v[j] < pivot) 
                swap(v[++i], v[j]);
            ++j;
        }
        swap(v[++i], v[rear]);
        quick_sort_partation(front, i-1); quick_sort_partation(i+1, rear);
        return;
    }

    void print_v(){
        for(auto vi : v){
            cout << vi << ",";
        }
        cout << endl;
    }

public:
    vector<int> v;
};