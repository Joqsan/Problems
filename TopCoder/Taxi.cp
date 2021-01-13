#include <vector>
#include <deque>
#include <utility>
#include <cmath>
#include <iostream>
#include <cstdio>

using namespace std;

typedef pair<int, int> position;

class Taxi
{
private:
    
    int speed;
    int time;
    int timePerSquare;
    int timeLimit;
    vector<position> taxis;
    vector<position> passengers;
    vector<int> from_taxi;
    vector<int> taxiMatchingPassenger;
    vector<int> passengerMatchingTaxi;
    
    
    bool PassengerReachableInTime(int p, int t);
    bool FindPath(int taxi_source);
public:
    
    Taxi(int s, int t, vector<position> taxisPos, vector<position> passPos);
    void maxPerson(void);
};

Taxi::Taxi(int s, int t, vector<position> taxisPos, vector<position> passPos) :
speed(s), time(t), taxis(taxisPos), passengers(passPos),
from_taxi(taxisPos.size(), -1),
taxiMatchingPassenger(taxisPos.size(), -1),
passengerMatchingTaxi(passPos.size(), -1)
{}

bool Taxi::PassengerReachableInTime(int p, int t)
{
    int manhattanDistance = abs(taxis[t].first - passengers[p].first) + abs(taxis[t].second - passengers[p].second);
    
    return manhattanDistance * timePerSquare <= timeLimit;
}

bool Taxi::FindPath(int taxi_source)
{
    int current_taxi = -1;
    int current_pass = -1;
    deque<int> Q;
    deque<int> forRematching;
    
    from_taxi[taxi_source] = -1;
    Q.push_back(taxi_source);
    bool path_found = false;
    
    while (!path_found && !Q.empty())
    {
        current_taxi = Q.front();
        Q.pop_front();
        
        for (int i = 0; i != passengers.size(); ++i)
        {
            current_pass = i;
            int matched_taxi = passengerMatchingTaxi[current_pass];
            
            if (PassengerReachableInTime(current_pass, current_taxi))
            {
                if (matched_taxi == -1)
                {
                    path_found = true;
                    break;
                }
                
                if (current_taxi != matched_taxi)
                    forRematching.push_back(matched_taxi);
            }
        }
        
        if (!path_found)
            while (!forRematching.empty())
            {
                if (from_taxi[forRematching.front()] == -1)
                {
                    from_taxi[forRematching.front()] = current_taxi;
                    Q.push_back(forRematching.front());
                }
                
                forRematching.pop_front();
            }
    }
    
    if (!path_found)
        return false;
    
    while (current_taxi != -1)
    {
        int temp_pass = taxiMatchingPassenger[current_taxi];
        taxiMatchingPassenger[current_taxi] = current_pass;
        passengerMatchingTaxi[current_pass] = current_taxi;
        
        current_taxi = from_taxi[current_taxi];
        current_pass = temp_pass;
    }
    
    return true;
}

void Taxi::maxPerson(void)
{
    int nPersons = 0;
    for (int i = 0; i != taxis.size(); ++i)
        nPersons += FindPath(i);
    
    cout << nPersons << endl;
}

void getData(vector<int> &data)
{
    int i = 0;
    while (i < 4 && scanf("%d", &data[i]) == 1)
        ++i;
}

void getPositions(vector<position> &objPos)
{
    int i = 0;
    while (i < objPos.size() && scanf("%d", &objPos[i].first) == 1)
    {
        scanf("%d", &objPos[i].second);
        ++i;
    }
}

int main(void)
{
    vector<int> data(4);
    getData(data);
    
    vector<position> passengers(data[0]);
    vector<position> taxis(data[1]);
    
    getPositions(passengers);
    getPositions(taxis);
    
    Taxi festival(data[2], data[3], taxis, passengers);
    
    festival.maxPerson();
    
    return 0;
}
