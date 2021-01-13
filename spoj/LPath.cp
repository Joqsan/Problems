#include <vector>
#include <deque>
#include <cstdio>

// Problem: PT07Z

using namespace std;

typedef pair<int, int> info;

class LongestPath
{
private:
    int V;
    vector<vector<int>> Adj;
    
    info BFS(int source);
public:
    LongestPath(int N);
    void CreateEdge(int u, int v);
    void FindLongestPath(void);
};

LongestPath::LongestPath(int N) :
V(N), Adj(N)
{
}

void LongestPath::CreateEdge(int u, int v)
{
    Adj[u].push_back(v);
    Adj[v].push_back(u);
}

info LongestPath::BFS(int source)
{
    deque<int> Q;
    vector<bool> visited(V, false);
    vector<int> d(V);
    int last;
    
    visited[source] = true;
    d[source] = 0;
    Q.push_back(source);
    
    while (!Q.empty())
    {
        int u = Q.front();
        Q.pop_front();
        
        last = u;
        
        for (auto v : Adj[u])
            if (!visited[v])
            {
                visited[v] = true;
                d[v] = d[u] + 1;
                Q.push_back(v);
            }
    }
    
    return {last, d[last]};
}

void LongestPath::FindLongestPath(void)
{
    info iPair = BFS(0);
    int distance = BFS(iPair.first).second;
    
    printf("%d\n", distance);
}

int main(void)
{
    int n;
    
    scanf("%d", &n);
    
    LongestPath game(n);
    
    for (int i = 0; i != n-1; ++i)
    {
        int u, v;
        scanf("%d %d", &u, &v);
        --u; --v;
        game.CreateEdge(u, v);
    }
    
    game.FindLongestPath();
    
    return 0;
    
}
