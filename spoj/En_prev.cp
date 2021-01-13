#include <vector>
#include <deque>

using namespace std;


class Trap
{
private:
    int V, s, t;
    vector<vector<int>> Adj;
    vector<vector<int>> cap;
    vector<int> counter;
    bool BFS(vector<int> &predecessor);
public:
    void getPlanet(void);
};

bool Trap::BFS(vector<int> &predecessor)
{
    deque<int> Q;
    vector<bool> visited(V, false);
    
    visited[s] = true;
    Q.push_back(s);
    
    while (!Q.empty())
    {
        int u = Q.front();
        Q.pop_front();
        
        for (auto v : Adj[u])
            if (visited[v] == false && cap[u][v] > 0)
            {
                visited[v] = true;
                predecessor[v] = u;
                Q.push_back(v);
            }
    }
    
    return predecessor[t] != -1;
}

void Trap::getPlanet(void)
{
    vector<int> predecessor;
    
    while (BFS(predecessor))
    {
        for (int v = t; v != s; v = predecessor[v])
        {
            int u = predecessor[v];
            
            --cap[u][v];
            ++cap[v][u];
            
        }
    }
}
