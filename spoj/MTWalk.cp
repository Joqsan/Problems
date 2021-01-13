#include <vector>
#include <deque>
#include <utility>
#include <cmath>
#include <algorithm>
#include <queue>
#include <cstdio>

using namespace std;

typedef pair<int, int> cell;
typedef priority_queue<pair<int, cell>, vector<pair<int, cell>>, greater<pair<int, cell>>> pQueue;
const int infty = 100 * 32767;

vector<int> dc = {0, -1, 1, 0};
vector<int> dr = {-1, 0, 0, 1};

bool IsInside(int V, int r, int c)
{
    return r >= 0 && r < V && c >= 0 && c < V;
}

class MTWalk
{
private:
    int V;
    vector<vector<int>> diffAtCell;
    vector<vector<cell>> minMaxAtCell;
    
public:
    MTWalk(int N);
    void FinMinDiff(const vector<vector<int>> Grid);
};

MTWalk::MTWalk(int N) :
V(N), diffAtCell(N, vector<int>(N, infty)), minMaxAtCell(N, vector<cell>(N, {infty, 0}))
{
}

void MTWalk::FinMinDiff(const vector<vector<int> > Grid)
{
    deque<cell> Q;
    vector<vector<bool>> allChildrenVisited(V, vector<bool>(V, false));
    vector<vector<pQueue>> advance(V, vector<pQueue>(V));
    vector<vector<cell>> predecessor(V, vector<cell>(V, {-1, -1}));
    
    
    int minTotalDiff = infty;
    int minHeigth = (Grid[V-1][V-1] < Grid[0][0]) ? Grid[V-1][V-1] : Grid[0][0];
    int maxHeigth = (minHeigth == Grid[0][0]) ? Grid[V-1][V-1] : Grid[0][0];
    int currentDiff = maxHeigth - minHeigth;
    diffAtCell[0][0] = currentDiff;
    minMaxAtCell[0][0] = {minHeigth, maxHeigth};
    
    Q.push_back({0, 0});
    
    while (!Q.empty())
    {
        int u1 = Q.front().first;
        int u2 = Q.front().second;
        
        if (u1 == V-1 && u2 == V-1)
        {
            minTotalDiff = min(minTotalDiff, diffAtCell[V-1][V-1]);
            Q.pop_front();
            continue;
        }
        
        if (allChildrenVisited[u1][u2])
        {
            if (!advance[u1][u2].empty())
            {
                int v1 = advance[u1][u2].top().second.first;
                int v2 = advance[u1][u2].top().second.second;
                
                if (diffAtCell[v1][v2] < minTotalDiff)
                {
                    allChildrenVisited[v1][v2] = false;
                    
                    minHeigth = minMaxAtCell[v1][v2].first;
                    maxHeigth = minMaxAtCell[v1][v2].second;
                    
                    currentDiff = advance[u1][u2].top().first;
                    
                    Q.push_front(advance[u1][u2].top().second);
                }
                
                advance[u1][u2].pop();
            }
            else
                Q.pop_front();
        }
        else
        {
            for (int i = 0; i != 4 && diffAtCell[u1][u2] < minTotalDiff; ++i)
            {
                int v1 = u1 + dr[i];
                int v2 = u2 + dc[i];
                
                if (IsInside(V, v1, v2) && diffAtCell[v1][v2] > diffAtCell[u1][u2])
                {
                    int minAtCell = min(minMaxAtCell[u1][u2].first, Grid[v1][v2]);
                    int maxAtCell = max(minMaxAtCell[u1][u2].second, Grid[v1][v2]);
                    
                    if (maxAtCell - minAtCell < diffAtCell[v1][v2])
                    {
                        diffAtCell[v1][v2] = maxAtCell - minAtCell;
                        minMaxAtCell[v1][v2] = {minAtCell, maxAtCell};
                        
                        if (maxAtCell - minAtCell <= currentDiff)
                        {
                            Q.push_front({v1, v2});
                            predecessor[v1][v2] = {u1, u2};
                        }
                        else if (maxAtCell - minAtCell < minTotalDiff)
                            advance[u1][u2].push({maxAtCell - minAtCell, {v1, v2}});
                    }
                }
            }
            
            allChildrenVisited[u1][u2] = true;
        }
    }
    
    printf("%d\n", minTotalDiff);
}

int main(void)
{
    int n;
    
    scanf("%d", &n);
    
    MTWalk game(n);
    
    vector<vector<int>> Grid(n, vector<int>(n));
    
    for (int i = 0; i != n; ++i)
        for (int j = 0; j != n; ++j)
        {
            int u;
            scanf("%d", &u);
            Grid[i][j] = u;
        }
    
    game.FinMinDiff(Grid);
    
    return 0;
    
}

