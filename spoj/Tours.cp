#include <vector>
#include <utility>
#include <queue>

using namespace std;

typedef pair<int, int> Node;

const int infty = 100 * 32767;

struct Edge
{
    int u, v, cap, cost, originalCost, prev;
};

class Graph
{
private:
    int V, nEdges, i;
    vector<Edge> Residual;
    vector<int> lastEdge;
    
    bool Dijkstra(vector<int> &predecessor, vector<int> &pi);
    void ReduceCost(const vector<int> &pi);
public:
    int s, t;
    Graph(int N, int M);
    void CreateEdge(int u, int v, int cap, int cost);
    void MinCost(void);
};

Graph::Graph(int N, int M) :
V(2*N + 2), nEdges(2*(M + 2*N)), s(0), t(V-1), i(0),
Residual(nEdges), lastEdge(V, -1)
{
}
void Graph::CreateEdge(int u, int v, int cap, int cost)
{
    Residual[i] = {u, v, cap, cost, cost, lastEdge[u]};
    lastEdge[u] = i++;
    
    Residual[i] = {v, u, 0, -cost, -cost, lastEdge[v]};
    lastEdge[v] = i++;
}

void Graph::ReduceCost(const vector<int> &pi)
{
    for (int e = 0; e != nEdges; ++e)
    {
        int u = Residual[e].u;
        int v = Residual[e].v;
        
        Residual[e].cost += pi[u] - pi[v];
    }
}

bool Graph::Dijkstra(vector<int> &inEdge, vector<int> &pi)
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
        
        if (visited[u])
            continue;
        
        visited[u] = true;
        
        for (int e = lastEdge[u]; e != -1; e = Residual[e].prev)
        {
            int v = Residual[e].v;
            
            if (Residual[e].cap > 0 && pi[v] > pi[u] + Residual[e].cost)
            {
                pi[v] = pi[u] + Residual[e].cost;
                inEdge[v] = e;
                Q.push({pi[v], v});
            }
        }
        
        
    }
    
    return inEdge[t] != -1;
}

void Graph::MinCost(void)
{
    vector<int> inEdge(V, -1), pi(V, infty);
    int min = 0;
    
    while (Dijkstra(inEdge, pi))
    {
        ReduceCost(pi);
        
        for (int v = t; v != s; v = Residual[inEdge[v]].u)
        {
            min += Residual[inEdge[v]].originalCost;
            --Residual[inEdge[v]].cap;
            ++Residual[inEdge[v]^1].cap;
        }
        
        fill(inEdge.begin(), inEdge.end(), -1);
        fill(pi.begin(), pi.end(), infty);
    }
    
    printf("%d\n", min);
}

int main(void)
{
    FILE *fp = fopen("Tours2.txt", "r");
    
    int nVertices, nEdges;
    
    fscanf(fp, "%d %d", &nVertices, &nEdges);
    
    Graph game(nVertices, nEdges);
    
    for (int i = 0; i != nEdges; ++i)
    {
        int u, v, w;
        fscanf(fp, "%d %d %d", &u, &v, &w);
        game.CreateEdge(u, v + nVertices, 1, w);
    }
    
    for (int v = 1; v != nVertices+1; ++v)
    {
        game.CreateEdge(game.s, v, 1, 0);
        game.CreateEdge(v+nVertices, game.t, 1, 0);
    }
    
    game.MinCost();
    
    return 0;
}
