#include <iostream>
#include <deque>
#include <vector>
#include <cstring>
#include <algorithm>
#include <ctime>
#include <cstdlib>
using namespace std;

const int LANDING = 6, RUNWAY = 4; // queue numbers
struct Landing {
    int id, fuel, time_stamp;
    bool operator<(Landing &_la) {
        return fuel < _la.fuel;
    } 
};
struct Takeoff {
    int id, time_stamp;
};
int even_cnt = 0, odd_cnt = 1;
int cur_time;
int land_sum, takeoff_sum;
int emerg_num, crash_num, land_num, takeoff_num;
int fuel_saved;

// handle incoming planes
void arrive(deque<Landing>&, deque<Takeoff>&); 
// determmine which planes can use the runways
void enter_runway(deque<Landing>[], deque<Takeoff>[], vector<int>&);
// print planes about to land
void landing_output(deque<Landing>&);
// print planes about to takeoff
void takeoff_output(deque<Takeoff>&);
// print the status of queues and runways
void output(deque<Landing>[], deque<Takeoff>[], vector<int>&);
// push incoming planes into corresponding queues
void push_into(deque<Landing>&, deque<Landing>[], deque<Takeoff>&, deque<Takeoff>[]);
// show statistics
void statistics();

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    srand(time(0));
    int time_unit = 500; // how much time unit to simulate
    deque<Landing> landing; // incoming planes about to land
    // planes in queue
    // [0, 1]: runway 2; [2, 3]: runway 3, [4, 5]: runway 4
    deque<Landing> landing_in_que[LANDING]; 
    deque<Takeoff> takeoff; // incoming planes about to takeoff
    deque<Takeoff> takeoff_in_que[RUNWAY]; // planes in queue
    // iff >> time_unit;
    for (cur_time=1; cur_time<=time_unit; ++cur_time) {
        vector<int> runways(4, -1);
        // Step 0:
        cout << "Simulation time: " << cur_time << "\n\n";
        output(landing_in_que, takeoff_in_que, runways);

        // Step 1:
        cout << "\nStep 1:\nLanding plane(s): ";
        arrive(landing, takeoff);
        landing_output(landing);
        cout << '\n';
        output(landing_in_que, takeoff_in_que, runways);

        // Step 2:
        cout << "\nStep 2:\nTakeoff plane(s): ";
        takeoff_output(takeoff);
        cout << '\n';
        output(landing_in_que, takeoff_in_que, runways);

        push_into(landing, landing_in_que, takeoff, takeoff_in_que);
        enter_runway(landing_in_que, takeoff_in_que, runways);
        output(landing_in_que, takeoff_in_que, runways);

        for (int i=0; i<LANDING; ++i) {
            for (auto &x: landing_in_que[i]) {
                --x.fuel;
            }
        }

        cout << "\n-----------------------------------\n";
    }
    statistics();
    return 0;
}

void arrive(deque<Landing> &la, deque<Takeoff> &ta) {
    int landing = rand() % 5; // no. of landing planes
    int takeoff = rand() % 5; // no. of takeoff planes
    for (int i=0; i<landing; ++i, odd_cnt+=2) {
        int fuel = rand() % 10 + 1;
        la.push_back(Landing{odd_cnt, fuel, cur_time});
    }
    
    for (int i=0; i<takeoff; ++i, even_cnt+=2) {
        ta.push_back(Takeoff{even_cnt, cur_time});
    }
}

void enter_runway(deque<Landing> la_in_q[], deque<Takeoff> ta_in_q[], vector<int> &run) {
    int run_cnt = 0;
    vector<Landing> emerg; // emergency: fuel = 0

    for (int i=0; i<LANDING; ++i) {
        for (int j=0, sz=la_in_q[i].size(); j<sz; ++j) {
            auto plane = la_in_q[i].front(); la_in_q[i].pop_front();
            if (plane.fuel == 0) 
                emerg.push_back(plane);
            else {
                la_in_q[i].push_back(plane);
            }
        }
    }

    // Step 3:
    cout << "\nStep 3:\nEmergency plane(s): ";
    for (auto &x: emerg) {
        cout << '(' << x.id << ',' << x.fuel << ')' << ", ";
    }
    cout << "\n\n";

    // emergency first
    for (auto &x: emerg) {
        run[run_cnt++] = x.id;
        ++land_num;
        ++emerg_num;
        land_sum += cur_time - x.time_stamp;
        if (run_cnt == RUNWAY) break;
    }
    output(la_in_q, ta_in_q, run);

    // Step 4:
    cout << "\nStep 4:\n\n";
    if (emerg.size() > RUNWAY) 
        crash_num += emerg.size() - RUNWAY; 

    // put planes onto runways depending on their numbers
    const int dev = 15; // give landing higher priority
    for (int i=run_cnt; i<RUNWAY; ++i) {
        int ft = 2*(i-1), sd = 2*i-1;
        if (!ta_in_q[i].empty() && cur_time - ta_in_q[i].front().time_stamp >= dev) {
            Takeoff to_takeoff = ta_in_q[i].front();
            run[i] = to_takeoff.id;
            ta_in_q[i].pop_front();

            ++takeoff_num;
            takeoff_sum += cur_time - to_takeoff.time_stamp;
        }
        else if (i > 0 && !la_in_q[ft].empty() || !la_in_q[sd].empty()) {
            Landing to_land;
            if (la_in_q[ft].size() >= la_in_q[sd].size()) {
                to_land = la_in_q[ft].front();
                run[i] = to_land.id;
                la_in_q[ft].pop_front();
            }
            else  {
                to_land = la_in_q[sd].front();
                run[i] = to_land.id;
                la_in_q[sd].pop_front();
            }
            ++land_num;
            land_sum += cur_time - to_land.time_stamp;
            fuel_saved += to_land.fuel;
        }
        else if (ta_in_q[i].size()) {
            Takeoff to_takeoff = ta_in_q[i].front();
            run[i] = to_takeoff.id;
            ta_in_q[i].pop_front();

            ++takeoff_num;
            takeoff_sum += cur_time - to_takeoff.time_stamp;
        }
    }
}

void landing_output(deque<Landing> &la) {
    for (auto &x: la) {
        cout << '(' << x.id << ", " << x.fuel << ')' << ", ";
    }
}

void takeoff_output(deque<Takeoff> &ta) {
    for (auto &x: ta) {
        cout << '(' << x.id  << ')' << ", ";
    }
}

void output(deque<Landing> la_in_q[], deque<Takeoff> ta_in_q[], vector<int> &run) {
    for (int i=0; i<RUNWAY; ++i) {
        cout << "Runway" << i+1 << '(' << run[i] << ")\n";

        for (int j=0; j<2; ++j) {
            cout << "Landing queue " << j+1 << ": ";
            if (i > 0) {
                landing_output(la_in_q[2*i-2+j]);
            }
            cout << '\n';
        }
        
        cout << "Takeoff queue: ";
        takeoff_output(ta_in_q[i]);
        cout << "\n\n";
    }
}

void push_into(deque<Landing> &la, deque<Landing> la_in_q[], deque<Takeoff> &ta, deque<Takeoff> ta_in_q[]) {
    // averagely distribute incoming planes
    sort(la.begin(), la.end()); // less fuel first
    int avg_size = 0;
    for (int i=0; i<LANDING; ++i) {
        avg_size += la_in_q[i].size();
    }
    avg_size /= LANDING;
    while (la.size()) {
        for (int i=LANDING-1; la.size()&&i>=0; i-=2) {
            if (la_in_q[i].size() <= avg_size && la_in_q[i-1].size() <= avg_size) {
                auto &pl = la_in_q[i].size() < la_in_q[i-1].size() ? la_in_q[i] : la_in_q[i-1];
                pl.push_back(la.front());
                la.pop_front();
            }
        }
        ++avg_size;
    }

    avg_size = 0;
    for (int i=0; i<RUNWAY; ++i) {
        avg_size += ta_in_q[i].size();
    }
    avg_size /= RUNWAY;
    while (ta.size()) {
        for (int i=0; ta.size()&&i<RUNWAY; ++i) {
            if (ta_in_q[i].size() <= avg_size) {
                ta_in_q[i].push_back(ta.front());
                ta.pop_front();
            }
        }
        ++avg_size;
    }
}

void statistics() {
    cout << "Time unit simulated: " << --cur_time << '\n';
    cout << "Average landing waiting time: " << land_sum / (double)land_num << "(s)\n";
    cout << "Average takeoff waiting time: " << takeoff_sum / (double)takeoff_num << "(s)\n";
    cout << "Average fuel saved: " << fuel_saved / (double)land_num << "(s)\n";
    cout << "Total plane(s) in emergency: " << emerg_num << '\n';
    cout << "Total plane(s) crashed: " << crash_num << '\n';
}