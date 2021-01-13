#include <vector>
#include <deque>
#include <algorithm>

using namespace std;

vector<int> dc = {1, 0, -1, 0};
vector<int> dr = {0,-1, 0, 1};

typedef pair<int, int> mountain;

bool IsInside(int R, int r, int C, int c)
{
    return r >= 0 && r < R && c >= 0 && c < C;
}

class MTWalk
{
private:
    int V, minHeight, maxHeight;
    
    
public:
    
    void BFS(const vector<vector<int>> &Grid);
};

void MTWalk::BFS(const vector<vector<int> > &Grid)
{
    deque<mountain> Q;
    vector<vector<bool>> visited(V, vector<bool>(V, false));
    
    visited[0][0] = true;
    Q.push_back({0, 0});
    
    while (!Q.empty())
    {
        int u1 = Q.front().first;
        int u2 = Q.front().second;
        Q.pop_front();
        
        for (int i = 0; i != 4; ++i)
        {
            int v1 = u1 + dr[i];
            int v2 = u2 + dc[i];
            
            if (IsInside(V, v1, V, v2) && !visited[u1][u2])
            {
                int minAtCell = 
            }
        }
    }
    
}
