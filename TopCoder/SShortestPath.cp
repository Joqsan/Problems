#include <vector>
#include <limits>
#include <utility>
#include <queue>

using namespace std;

int infty = numeric_limits<short>::max();

typedef pair<int, int> b;

struct Edge
{
    int to;
    int fwd_cap;
    int fwd_cost;
    int bwd_cap;
    int bwd_cost;
};


struct Node
{
    int vertex;
    int *distance;
    
    Node() {};
};

bool SortByWeight(const Node &a, const Node &b)
{
    return *a.distance > *b.distance;
}

class Graph
{
private:
    int V;
    int s;
    int t;
    // {to, cost}
    vector<vector<int>> Adj;
    vector<vector<int>> capacities;
    vector<vector<int>> costs;
    
    void BellmanFord(int s, vector<int> &pi);
    bool Dijsktra(int s, int t, vector<int> &predecessor, vector<int> &pi);
    void ReduceCost(const vector<int> &pi);
    
public:
    Graph(int n);
    void SShortestPath(int Vertices);
    
};


Graph::Graph(int n) :
Adj(n), s(0), t(n-1)
{
}



// Assume there are no negative cycles in G.
void Graph::BellmanFord(int s, vector<int> &pi)
{
    pi[s] = 0;
    
    for (int i = 0; i != V-1;++i)
        for (int u = 0; u != V; ++u)
            for (auto edge : Adj[u])
            {
                const int &v = edge.to;
                const int &w = edge.cost;
                
                if (pi[v] > pi[u] + w)
                    pi[v] = pi[u] + w;
            }
}

bool Graph::Dijsktra(int s, int t, vector<int> &predecessor, vector<int> &pi)
{
    priority_queue<Node, vector<Node>, decltype(SortByWeight)*> Q(SortByWeight);
    
    pi[s] = 0;
    predecessor[s] = -1;
    
    for (int u = 0; u != V; ++u)
        Q.emplace(u, &pi[u]);
    
    while (!Q.empty())
    {
        // node = {v, delta(s,v) = pi[v]}
        const int &u = Q.top().vertex;
        
        for (auto v : Adj[u])
        {
            const int &w = costs[u][v];
            
            if (pi[v] > pi[u] + w)
            {
                pi[v] = pi[u] + w;
                predecessor[v] = u;
            }
        }
        
        Q.pop();
    }
    
    return predecessor[t] != -1;
}

void Graph::ReduceCost(const vector<int> &pi)
{
    for (int i = 0; i != V; ++i)
        for (auto j : Adj[i])
        {
            costs[i][j] += pi[i] - pi[j];
            costs[j][i] = -costs[i][i];
        }
}

void Graph::SShortestPath()
{
    vector<vector<int>> flows(V);
    
    vector<int> pi(V, infty);
    BellmanFord(0, pi);
    
    ReduceCost(pi);
    
    vector<int> predecessor(V, -1);
    while (Dijsktra(s, t, predecessor, pi))
    {
        int v = t;
        int c_fp = infty;
        while (v != -1)
        {
            c_fp = min(c_fp, cost[predecessor[v]][v]);
        }
    }
    
}
