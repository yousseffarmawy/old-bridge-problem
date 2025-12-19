#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <atomic>
#include <random>
using namespace std;
///////
class Bridge{
public:
using clk = chrono::steady_clock;
static const int left = 0;
 static const int right = 1;
Bridge():
 cars_on_bridge(0),waiting{false,false},current_dir(-1){}
unique_lock<mutex>get_lock()
{

    return unique_lock<mutex>(mtx);
}
void mark_wating_unlock(int dir )
{
  waiting[dir]++;
  if (!first_wait_Set[dir]){ 

    first_wait_time[dir] = clk::now();
    first_wait_Set[dir] = true;
  }

}
void clear_wating_unlock(int dir)
{
if (waiting[dir] > 0) --waiting[dir];

    if( waiting[dir]==0)  first_wait_Set[dir] = false;
    
}
bool can_enter_unlock(int dir)
{
    if (cars_on_bridge == 0)
    {
        if (waiting[left]>0 &&waiting[right]>0)
     {
if (first_wait_time[left]<=first_wait_time[right])
return dir == left;
else return dir == right  ;
}
else if (waiting[left]>0){
return dir == left;
}
else if (waiting[right]>0){
    return dir == right;

} 
else return true ;
}
else 
{
    return (current_dir == dir)&&(cars_on_bridge < 3);
}
}
void enter_unlocked(int dir)
{
if (cars_on_bridge == 0) current_dir = dir;
   ++cars_on_bridge;
}

void exit_unlocked()
{

if (cars_on_bridge>0) --cars_on_bridge;
if (cars_on_bridge == 0)
{
    current_dir = -1;
}
}
void notify_all()
{

cv.notify_all();

}
void notify_one()
{

cv.notify_one();
}
////////////

int get_cars_on_bridge_unlocked() const { return cars_on_bridge; }
int get_waiting_unlocked(int dir) const { return waiting[dir]; }
int get_current_dir_unlocked() const { return current_dir; }

//////////////

condition_variable& condition_var() 
{ 
    return cv;
 }

void arrive_and_across(Bridge &bridge, int id, int dir)
{
    {
        auto lock = bridge.get_lock();
        bridge.mark_wating_unlock(dir);

        bridge.condition_var().wait(lock, [&]() {
            return bridge.can_enter_unlock(dir);
        });

        bridge.clear_wating_unlock(dir);
        bridge.enter_unlocked(dir);

        cout << "car" << id
             << " enter the bridge side "
             << (dir == Bridge::left ? "left" : "right")
             << "| on_bridge=" << bridge.get_cars_on_bridge_unlocked()
             << endl;
    }

    this_thread::sleep_for(chrono::milliseconds(500 + (id % 5) * 200));

    {
        auto lock2 = bridge.get_lock();
        bridge.exit_unlocked();

        cout << "[EXIT ] Car " << id
             << " dir=" << (dir == Bridge::left ? "LEFT" : "RIGHT")
             << " | on_bridge=" << bridge.get_cars_on_bridge_unlocked()
             << "\n";

        bridge.notify_all();
    }
}





private:
mutex mtx;
condition_variable cv;
int cars_on_bridge;
int waiting [2];
bool first_wait_Set[2];
clk::time_point first_wait_time[2];
int current_dir;

////////


};
int main() {
    Bridge bridge;

    const int N = 12;
    vector<thread> threads;
    threads.reserve(N);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dirDist(0,1);
    uniform_int_distribution<int> jitter(0, 200); 

    for (int i = 0; i < N; ++i) {
        int dir = dirDist(gen); 
        int delay = jitter(gen);
        this_thread::sleep_for(chrono::milliseconds(delay));
        threads.emplace_back([&bridge, i, dir]() {
            bridge.arrive_and_across(bridge, i+1, dir);
        });
    }


    for (auto &t : threads) t.join();

    cout << "Simulation finished.\n";
    return 0;
}
