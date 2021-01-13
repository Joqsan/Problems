#include <vector>
#include <deque>
#include <cstdio>
#include <limits>
#include <cstdio>

using namespace std;

int infty = numeric_limits<short>::max();

class PotHole
{
private:
    
    int V;
    int s;
    int t;
    int maxCap;
    vector<vector<int>> Adj;
    vector<vector<int>> cap;
    bool BFS(vector<int> &predecessor);
public:
    PotHole(int n);
    void getCap(int m);
    void CreateEdge(int u, int v);
    void getMaxPeople(void);
};

PotHole::PotHole(int n) :
V(n), s(0), t(n-1), Adj(n), cap(n, vector<int>(n, infty))
{
}

void PotHole::getCap(int m)
{
    maxCap = m;
}

void PotHole::CreateEdge(int u, int v)
{
    Adj[u].push_back(v);
    Adj[v].push_back(u);
    
    if (u == s || v == t)
        cap[u][v] = 1;
    else
        cap[u][v] = maxCap;
    
    cap[v][u] = 0;
}

bool PotHole::BFS(vector<int> &predecessor)
{
    vector<bool> visited(V, false);
    
    deque<int> Q;
    
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

void PotHole::getMaxPeople(void)
{
    vector<int> predecessor(V, -1);
    int maxFlow = 0;
    
    while (BFS(predecessor))
    {
        ++maxFlow;
        for (int v = t; v != s; v = predecessor[v])
        {
            const int &u = predecessor[v];
            
            // c_fp will always be 1 so we add and subtract
            // one unit of flow along the path
            --cap[u][v];
            ++cap[v][u];
        }
        
        predecessor.clear(); predecessor.resize(V, -1);
    }
    
    printf("%d\n", maxFlow);
}

int main(void)
{
    int nCases;
    scanf("%d", &nCases);
    while (nCases--)
    {
        int n;
        scanf("%d", &n);
        PotHole game(n);
        
        for (int u = 0; u != n-1; ++u)
        {
            int m, v, i;
            scanf("%d", &m);
            
            if (u == 0)
                game.getCap(m);
            
            i = 0;
            while (i != m && scanf("%d", &v) == 1)
            {
                game.CreateEdge(u, v-1);
                ++i;
            }
        }
        
        game.getMaxPeople();
    }
    
    return 0;
}
