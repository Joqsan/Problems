#include <vector>
#include <algorithm>
#include <limits>
#include <cstdio>
#include <set>

using namespace std;

const long infty = numeric_limits<long>::max();

class FastFlow
{
private:
    int V, s, t;
    vector<set<int>> Adj;
    vector<vector<long>> cap;
    vector<int> levels;
    
    bool BFS(void);
    long PushFlow(const int &u, long flowReachingU);
public:
    
    FastFlow(int N, int M);
    void CreateEdge(int u, int v, long cap);
    void MaxFlow(void);
    
};

FastFlow::FastFlow(int N, int M) :
V(N), s(0), t(V-1), Adj(V), cap(V, vector<long>(V, 0)), levels(V)
{
}

void FastFlow::CreateEdge(int u, int v, long capacity)
{
    Adj[u].insert(v); Adj[v].insert(u);
    cap[u][v] = cap[v][u] += capacity;
}

bool FastFlow::BFS(void)
{
    vector<int> Q(V);
    int qh(0), qt(0);
    vector<bool> visited(V, false);
    
    visited[s] = true;
    levels[s] = 0;
    
    Q[qt++] = s;
    
    while (qh != qt)
    {
        int u = Q[qh++];
        
        for (auto v : Adj[u])
            if (!visited[v] && cap[u][v] > 0)
            {
                visited[v] = true;
                levels[v] = levels[u] + 1;
                Q[qt++] = v;
            }
    }
    
    return visited[t];
}

long FastFlow::PushFlow(const int &u, long flowReachingU)
{
    long flowLeavingU = 0;
    
    if (u == t)
        return flowReachingU;
    
    for (auto v : Adj[u])
    {
        if (flowReachingU > 0)
        {
            if (levels[v] == levels[u] + 1 && cap[u][v] > 0)
            {
                long flowPushed = min(flowReachingU, cap[u][v]);
                long total_cfp = PushFlow(v, flowPushed);
                
                if (total_cfp)
                {
                    flowReachingU -= total_cfp;
                    flowLeavingU += total_cfp;
                    
                    cap[u][v] -= total_cfp;
                    cap[v][u] += total_cfp;
                }
            }
        }
        else
            break;
    }
    
    return flowLeavingU;
}

void FastFlow::MaxFlow(void)
{
    long max = 0;
    while (BFS())
        max += PushFlow(s, infty);
    
    printf("%ld\n", max);
}

int main(void)
{
    int N, M;
    scanf("%d %d", &N, &M);
    
    FastFlow game(N, M);
    
    for (int i = 0; i != M; ++i)
    {
        int u, v;
        long cap;
        scanf("%d %d %ld", &u, &v, &cap);
        --u; --v;
        if (u != v)
            game.CreateEdge(u, v, cap);
    }
    
    game.MaxFlow();
    
    return 0;
}
