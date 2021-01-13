#include <vector>
#include <queue>
#include <utility>
#include <cstdio>
#include <algorithm>

using namespace std;

int infty = 100 * 32767;
typedef pair<int, int> Node;

class Assignment
{
private:
    int V;
    vector<vector<int>> cost;
    vector<vector<int>> originalCost;
    vector<vector<int>> cap;
    
    void BellmanFord(vector<int> &pi);
    void ReduceCost(const vector<int> &pi);
    bool Dijkstra(vector<int> &predecessor, vector<int> &pi);
public:
    
    int s, t;
    void CreateEdge(int u, int v, int cap, int cost);
    Assignment(int m, int n);
    void MinCost(void);
};

Assignment::Assignment(int m, int n) :
s(0), V(m+n+2), t(V-1),
cost(V, vector<int>(V, infty)), cap(V, vector<int>(V, 0))
{
}

void Assignment::CreateEdge(int u, int v, int edgeCap, int edgeCost)
{
    cost[u][v] = edgeCost;
    cost[v][u] = -cost[u][v];
    
    cap[u][v] = edgeCap;
    cap[v][u] = 0;
}

void Assignment::BellmanFord(vector<int> &pi)
{
    pi[s] = 0;
    
    for (int i = 0; i != V-1; ++i)
        for (int u = 0; u != V; ++u)
            for (int v = 0; v != V; ++v)
                if (v != u && pi[v] > pi[u] + cost[u][v] && cap[u][v] > 0)
                    pi[v] = pi[u] + cost[u][v];
}

void Assignment::ReduceCost(const vector<int> &pi)
{
    for (int u = 0; u != V; ++u)
        for (int v = 0; v != V; ++v)
            if (v != u)
                cost[u][v] += pi[u] - pi[v];
}

bool Assignment::Dijkstra(vector<int> &predecessor, vector<int> &pi)
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

void Assignment::MinCost(void)
{
    originalCost = cost;
    
    int min = 0;
    vector<int> predecessor(V, -1);
    vector<int> pi(V, infty);
    
    BellmanFord(pi);
    ReduceCost(pi);
    
    fill(predecessor.begin(), predecessor.end(), -1);
    fill(pi.begin(), pi.end(), infty);
    while (Dijkstra(predecessor, pi))
    {
        ReduceCost(pi);
        
        for (int v = t; v != s; v = predecessor[v])
        {
            const int &u = predecessor[v];
            --cap[u][v];
            ++cap[v][u];
            min += originalCost[u][v];
        }
        
        fill(predecessor.begin(), predecessor.end(), -1);
        fill(pi.begin(), pi.end(), infty);
    }
    
    printf("%d\n", min);
}

int main(void)
{
    int nCases, m, n;
    
    scanf("%d", &nCases);
    
    while (nCases--)
    {
        scanf("%d %d", &m, &n);
        Assignment game(m, n);
        
        for (int i = 1; i != m+1; ++i)
        {
            int u;
            scanf("%d", &u);
            game.CreateEdge(game.s, i, u, 0);
        }
        
        for (int j = 1; j != n+1; ++j)
        {
            int u;
            scanf("%d", &u);
            game.CreateEdge(j+m, game.t, u, 0);
        }
        
        for (int i = 1; i != m+1; ++i)
            for (int j = 1; j != n+1; ++j)
            {
                int u;
                scanf("%d", &u);
                game.CreateEdge(i, j+m, infty, u);
            }
        
        game.MinCost();
    }
    
    return 0;
}


