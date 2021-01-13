#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>
#include <cstdio>

using namespace std;

typedef pair<long, long> Position;

class Taxi
{
private:
    
    int L, R;
    
    vector<int> TmatchingP, PmatchingT;
    vector<bool> visited;
    
    bool Match(int source);
public:
    
    vector<vector<int>> Adj;
    Taxi(int t, int p);
    void MaxPassengers(void);
    
};

Taxi::Taxi(int t, int p) :
L(t), R(p), Adj(L, vector<int>(R, 0)), TmatchingP(t, -1),
PmatchingT(p, -1), visited(L)
{
}

bool PassengerReachable(const Position pass, const Position taxi, int speed, int timeLimit)
{
    long distance = 200 * (abs(pass.first - taxi.first) + abs(pass.second - taxi.second));
    
    return distance <= speed*timeLimit;
}

bool Taxi::Match(int current_passenger)
{
    for (int current_taxi = 0; current_taxi != L; ++current_taxi)
        if (!visited[current_taxi] &&  Adj[current_taxi][current_passenger] > 0)
        {
            visited[current_taxi] = true;
            
            if (TmatchingP[current_taxi] == -1 || Match(TmatchingP[current_taxi]))
                {
                    TmatchingP[current_taxi] = current_passenger;
                    PmatchingT[current_passenger] = current_taxi;
                    return true;
                }
        }
    
    return false;
}

void Taxi::MaxPassengers(void)
{
    int maxPass = 0;
    for (int passenger = 0; passenger != R; ++passenger)
    {
        fill(visited.begin(), visited.end(), false);
        maxPass += Match(passenger);
    }
    
    printf("%d\n", maxPass);
}

int main(void)
{
    int nCases, p, t, s, c;
    
    scanf("%d", &nCases);
    
    while (nCases--)
    {
        scanf("%d %d %d %d", &p, &t, &s, &c);
        
        Taxi game(t, p);
        vector<Position> T(t);
        vector<Position> P(p);
        
        for (int j = 0; j != p; ++j)
        {
            long x, y;
            scanf("%ld %ld", &x, &y);
            P[j] = {x, y};
        }
        
        for (int i = 0; i != t; ++i)
        {
            long x, y;
            scanf("%ld %ld", &x, &y);
            T[i] = {x, y};
        }
        
        for (int i = 0; i != t; ++i)
            for (int j = 0; j != p; ++j)
                if (PassengerReachable(P[j], T[i], s, c))
                    game.Adj[i][j] = 1;
        
        game.MaxPassengers();
                    
    }
}

