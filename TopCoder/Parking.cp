#include <vector>
#include <string>
#include <utility>
#include <deque>
#include <map>
#include <algorithm>
#include <iostream>

using namespace std;

bool IsInside(int r, int R, int c, int C)
{
    return r >= 0 && r < R && c >= 0 && c < C;
}

typedef pair<int, int> Node;


class Parking
{
private:
    
    int minimal = 0;
    
    vector<Node> cars;
    vector<Node> slots;
    map<Node, int> idxSlot;
    vector<string> padded_park;
    
    // pair<which_slot, cost>
    vector<vector<pair<int, int>>> AllCosts;
    
    vector<int> slotMatchingCar;
    vector<int> carMatchingSlot;
    vector<int> from_car;
    
    bool FindPathCost(int car_source);
    bool FindCarsSlots(vector<string> park);
    bool Matching(int car_source);
    
public:
    
    int minTime(vector<string> park);
    
    Parking() {};
    
};

vector<int> dx = {0, -1, 0, 1};
vector<int> dy = {-1, 0, 1, 0};

bool Parking::FindCarsSlots(vector<string> park)
{
    //padded park
    padded_park.push_back(string(park[0].size() + 2, 'X'));
    for (int i = 0; i != park.size(); ++i)
        padded_park.push_back("X" + park[i] + "X");
    padded_park.push_back(string(park[0].size() + 2, 'X'));

    for (int i = 0; i != padded_park.size(); ++i)
        for (int j = 0; j != padded_park[0].size(); ++j)
        {
            if (padded_park[i][j] == 'C')
                cars.push_back({i, j});
            else if (padded_park[i][j] == 'P')
                slots.push_back({i, j});
        }
    
    if (cars.size() > slots.size())
        return false;
    
    slotMatchingCar.resize(slots.size(), -1);
    carMatchingSlot.resize(cars.size(), -1);
    from_car.resize(cars.size());
    
    int i = 0;
    for (auto slot : slots)
    {
        idxSlot[slot] = i;
        ++i;
    }
    
    return true;
}

bool Parking::FindPathCost(int idx_car)
{
    vector<string> parkBFS(padded_park);
    int R = int(parkBFS.size());
    int C = int(parkBFS[0].size());
    
    vector<vector<Node>> from_node(padded_park.size(), vector<Node>(padded_park[0].size(), {-1, -1}));
    vector<Node> slots_found;   // ordered from neares to farthest
    deque<Node> Q;
    

    parkBFS[cars[idx_car].first][cars[idx_car].second] = ' ';
    Q.push_back(cars[idx_car]);
    while (!Q.empty())
    {
        auto x = Q.front().first;
        auto y = Q.front().second;
        Q.pop_front();
        for (int j = 0; j != 4; ++j)
        {
            if (IsInside(x + dx[j], R, y + dy[j], C) &&
                (parkBFS[x + dx[j]][y + dy[j]] == 'P' || parkBFS[x + dx[j]][y + dy[j]] == '.'))
            {
                if (parkBFS[x + dx[j]][y + dy[j]] == 'P')
                    slots_found.push_back({x + dx[j], y + dy[j]});
                
                from_node[x + dx[j]][y + dy[j]] = {x, y};
                
                parkBFS[x + dx[j]][y + dy[j]] = ' ';
                Q.push_back({x + dx[j], y + dy[j]});
            }
        }
    }
    
    if (slots_found.empty())
        return false;
    
    // find the cost from the car to each parking slot
    // inf slot_index slot appear from the nearest to the farthest
    // wrt to the car
    // thisCarCosts = (coordinates, cost);
    vector<pair<int, int>> thisCarCosts;
    for (auto node : slots_found)
    {
        Node current_node = node;
        int cost = -1;
        while (current_node.first != -1)
        {
            ++cost;
            current_node = from_node[current_node.first][current_node.second];
        }
        
        thisCarCosts.push_back({idxSlot[{node.first, node.second}], cost});
        
    }
    
    
    AllCosts.push_back(thisCarCosts);
    
    return true;
}

bool Parking::Matching(int car_source)
{
    int current_car = 0;
    int current_slot = 0;
    deque<int> Q;
    
    from_car[car_source] = -1;
    Q.push_back(car_source);
    bool path_found = false;
    
    while (!path_found && !Q.empty())
    {
        current_car = Q.front();
        for (auto slot_cost : AllCosts[current_car])
        {
            current_slot = slot_cost.first;
            int matched_car = slotMatchingCar[current_slot];
            
            if (slot_cost.second <= minimal)
            {
                if (current_car != matched_car && matched_car == -1)
                {
                    path_found = true;
                    Q.pop_front();
                    break;
                }
                
            }
            else
            {
                minimal = slot_cost.second;
                
                if (matched_car != -1 && from_car[matched_car] == -1)
                {
                    from_car[matched_car] = current_car;
                    Q.push_back(matched_car);
                    Q.pop_front();
                }
                
                break;
            }
        }
    }
    
    if (!path_found)
        return false;
    
    while (current_car != -1)
    {
        int temp_slot = carMatchingSlot[current_car];
        carMatchingSlot[current_car] = current_slot;
        slotMatchingCar[current_slot] = current_car;
        
        current_car = from_car[current_car];
        current_slot = temp_slot;
    }
    
    return true;
}

int Parking::minTime(vector<string> park)
{
    int n_carReachesSlot = 0;
    if (FindCarsSlots(park))
        for (int i = 0; i != cars.size(); ++i)
            n_carReachesSlot += FindPathCost(i);
    
    if (n_carReachesSlot < cars.size())
        return -1;
    
    int n_matchedCars = 0;
    for (int i = 0; i != cars.size(); ++i)
        n_matchedCars += Matching(i);
    
    if (n_matchedCars < cars.size())
        return -1;
    
    return minimal;
}

int main(void)
{
    Parking park;
    
    int min= park.minTime(
    {   "C...P.......",
        "..X.........",
        ".XX.........",
        ".X..........",
        ".X..........",
        ".X..........",
        "PX..C......P"
    });
    
    cout << min << endl;
    
    return 0;
}
