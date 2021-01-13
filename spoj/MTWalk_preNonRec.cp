#include <vector>
#include <deque>
#include <utility>
#include <cmath>
#include <algorithm>

using namespace std;

typedef pair<int, int> cell;
const int infty = 100 * 32767;

vector<int> dc = {1, 0, -1, 0};
vector<int> dr = {0, -1, 0, 1};

bool IsInside(int V, int r, int c)
{
    return r >= 0 && r < V && c >= 0 && c < V;
}

class MTWalk
{
private:
    int V;
    
public:
    
    void FinMinDiff(const vector<vector<int>> Grid);
};


void MTWalk::FinMinDiff(const vector<vector<int> > Grid)
{
    deque<cell> Q;
    vector<vector<bool>> visited(V, vector<bool>(V, false)), allChildrenVisited(V, vector<bool>(V, false));
    vector<vector<cell>> predecessor(V, vector<cell>(V, {-1, -1}));
    vector<vector<pair<int, cell>>> minDiffAdvance(V, vector<pair<int, cell>>(V, {infty, {infty, infty}}));
    
    int minPath, maxPath;
    minPath = maxPath = Grid[0][0];
    int currentDiff = 0;
    
    Q.push_back({0, 0});
    
    
    while (!Q.empty())
    {
        int u1 = Q.front().first;
        int u2 = Q.front().second;
        
        if (allChildrenVisited[u1][u2])
            Q.pop_front();
        else
        {
            for (int i = 0; i != 4; ++i)
            {
                int v1 = u1 + dr[i];
                int v2 = u2 + dc[i];
                
                if (IsInside(V, v1, v2) && predecessor[u1][u2] != make_pair(v1, v2))
                {
                    int diff = max(abs(Grid[v1][v2] - minPath), abs(Grid[v1][v2] - maxPath));
                    
                    if (diff <= currentDiff)
                    {
                        Q.push_front({v1, v2});
                        advancePath[v1][v2] = true;
                    }
                    else
                        minDiffAdvance[u1][u2] = min(minDiffAdvance[u1][u2], {diff, {v1, v2}});
                }
            }
        }
    }
    
    
}
