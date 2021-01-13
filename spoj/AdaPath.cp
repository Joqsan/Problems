#include <vector>
#include <deque>
#include <utility>
#include <algorithm>
#include <cstdio>

using namespace std;

typedef pair<int, int> cell;

vector<int> dc = {1, 0, -1, 0};
vector<int> dr = {0, 1, 0, -1};

bool IsInside(int N, int r, int c)
{
    return r >= 0 && r < N && c >= 0 && c < N;
}

class AdaPath
{
private:
    int V;
    vector<vector<bool>> inPath;
    
    cell BFS(const vector<vector<int>> &Grid, const cell &s, vector<vector<cell>> &predecessor);
public:
    
    AdaPath(int N);
    void FindValidGame(const vector<vector<int>> &Grid, deque<cell> &oneSet);
};

AdaPath::AdaPath(int N) :
V(N), inPath(N, vector<bool>(N, false))
{
}

cell AdaPath::BFS(const vector<vector<int>> &Grid, const cell &s, vector<vector<cell>> &predecessor)
{
    deque<cell> Q;
    vector<vector<bool>> visited(V, vector<bool>(V, false));
    
    cell last;
    
    visited[s.first][s.second] = true;
    Q.push_back(s);
    
    while (!Q.empty())
    {
        int u1 = Q.front().first;
        int u2 = Q.front().second;
        
        last = Q.front();
        Q.pop_front();
        
        for (int i = 0; i != 4; ++i)
        {
            int v1 = u1 + dr[i];
            int v2 = u2 + dc[i];
            
            if (IsInside(V, v1, v2) && !visited[v1][v2] && Grid[v1][v2] == Grid[u1][u2]+1)
            {
                if (!inPath[v1][v2])
                {
                    visited[v1][v2] = true;
                    predecessor[v1][v2] = {u1, u2};
                    Q.push_back({v1, v2});
                }
            }
        }
    }
    
    return last;
}


void AdaPath::FindValidGame(const vector<vector<int>> &Grid, deque<cell> &oneSet)
{
    int numCell = 0;
    while (!oneSet.empty())
    {
        cell one = oneSet.front();
        oneSet.pop_front();
        
        vector<vector<cell>> predecessor(V, vector<cell>(V, {-1, -1}));
        cell last = BFS(Grid, one, predecessor);
        
        while (last.first != -1 && last.second != -1)
        {
            ++numCell;
            inPath[last.first][last.second] = true;
            int tempFirst = last.first;
            last.first = predecessor[tempFirst][last.second].first;
            last.second = predecessor[tempFirst][last.second].second;
        }
    }
    
    if (numCell == V*V)
        printf("YES\n");
    else
        printf("NO\n");
}

int main(void)
{
    int nCases, n;
    
    scanf("%d", &nCases);
    
    while (nCases--)
    {
        scanf("%d", &n);
        
        vector<vector<int>> Grid(n, vector<int>(n));
        deque<cell> oneSet;
        
        AdaPath game(n);
        
        for (int i = 0; i != n; ++i)
            for (int j = 0; j != n; ++j)
            {
                int u;
                scanf("%d", &u);
                
                if (u == 1)
                    oneSet.push_back({i, j});
                
                Grid[i][j] = u;
            }
        
        game.FindValidGame(Grid, oneSet);
    }
    
    return 0;
}
