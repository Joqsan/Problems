#include <vector>
#include <queue>
#include <utility>
#include <cstdio>

using namespace std;

int infty = 100 * 32767;
typedef pair<int, int> Node;

class Selfish
{
private:
    int V, s, t, L, R;
    vector<vector<int>> cost;
    vector<vector<int>> originalCost;
    vector<vector<int>> cap;
    
    void BellmanFord(vector<int> &pi);
    void ReduceCost(const vector<int> &pi);
    bool Dijkstra(vector<int> &predecessor, vector<int> &pi);
public:
    
    Selfish(int l, int r);
    int getMaxGoods(vector<vector<int>> rawCost);
};

Selfish::Selfish(int l, int r) :
L(l), R(r), s(0), V(l+r+2), t(V-1),
cost(V, vector<int>(V, infty)), cap(V, vector<int>(V, 0))
{
}

void Selfish::BellmanFord(vector<int> &pi)
{
    pi[s] = 0;
    
    for (int i = 0; i != V-1; ++i)
        for (int u = 0; u != V; ++u)
            for (int v = 0; v != V; ++v)
                if (v != u && pi[v] > pi[u] + cost[u][v] && cap[u][v] > 0)
                    pi[v] = pi[u] + cost[u][v];
}

void Selfish::ReduceCost(const vector<int> &pi)
{
    for (int u = 0; u != V; ++u)
        for (int v = 0; v != V; ++v)
            if (v != u)
                cost[u][v] += pi[u] - pi[v];
}

bool Selfish::Dijkstra(vector<int> &predecessor, vector<int> &pi)
{
    priority_queue<Node, vector<Node>, greater<Node>> Q;
    vector<bool> visited(V, false);
    
    pi[s] = 0;
    
    Q.push({pi[s], s});
    
    while (!Q.empty())
    {
        int u = Q.top().second;
        Q.pop();
        
        if (u == t)
            break;
        
        if (visited[u] == true)
            continue;
        
        visited[u] = true;
        
        for (int v = 0; v != V; ++v)
            if (v != u && cap[u][v] > 0)
                if (pi[v] > pi[u] + cost[u][v])
                {
                    pi[v] = pi[u] + cost[u][v];
                    predecessor[v] = u;
                    Q.push({pi[v], v});
                }
    }
    
    return predecessor[t] != -1;
    
}

int Selfish::getMaxGoods(vector<vector<int>> rawCost)
{
    // Creating network
    // Connecting every v in L with s (cap = 1, cost = 0)
    for (int u = 1; u != L+1; ++u)
    {
        cap[s][u] = 1   ;
        cost[s][u] = cost[u][s] = 0;
    }
    
    // Connecting every u in L with every in v in R (cap = 1, cost = rawcost)
    for (int u = 1; u != L+1; ++u)
        for (int v = 1; v != R+1; ++v)
        {
            if (rawCost[u-1][v-1] < infty)
            {
                cap[u][L+v] = 1;
                cost[u][L+v] = -rawCost[u-1][v-1];
                cost[L+v][u] = -cost[u][L+v];
            }
        }
    
    // Connecting every v in R with t (cap = 1, cost = 0)
    for (int v = 1; v != R+1; ++v)
    {
        cap[v+L][t] = 1;
        cost[v+L][t] = cost[t][L+v] = 0;
    }
    
    originalCost = cost;
    
    int maxGoods = 0;
    vector<int> predecessor(V, -1);
    vector<int> pi(V, infty);
    
    BellmanFord(pi);
    ReduceCost(pi);
    
    predecessor.clear(); pi.clear();
    predecessor.resize(V, -1); pi.resize(V, infty);
    while (Dijkstra(predecessor, pi))
    {
        ReduceCost(pi);
        
        for (int v = t; v != s; v = predecessor[v])
        {
            const int &u = predecessor[v];
            --cap[u][v];
            ++cap[v][u];
            maxGoods += -originalCost[u][v];
        }
        
        predecessor.clear(); pi.clear();
        predecessor.resize(V, -1); pi.resize(V, infty);
    }
    
    return maxGoods;
}

int main(void)
{
    int nCases, L, R, u, v, c;
    
    scanf("%d", &nCases);
    while (nCases--)
    {
        scanf("%d %d", &L, &R);
        Selfish cities(L, R);
        
        vector<vector<int>> rawCost(L, vector<int>(R, infty));
        while (scanf("%d %d %d", &u, &v, &c) == 3)
        {
            if (u == 0)
                break;
            
            rawCost[u-1][v-1] = c;
        }
        
        int maxGoods = cities.getMaxGoods(rawCost);
        
        printf("%d\n", maxGoods);
    }
    
    return 0;
    
}
