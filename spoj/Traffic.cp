#include <vector>
#include <utility>
#include <queue>
#include <algorithm>
#include <cstdio>
#include <limits>

using namespace std;

typedef long long lli;

typedef pair<lli, int> Node;

const lli infty = numeric_limits<long>::max();

struct Edge
{
    int u, v, weight, prev;
};

class Traffic
{
private:
    int V, s, t, k;
    
    vector<Edge> twoWayRoads;
    
    vector<Edge> Graph, transposeGraph;
    vector<int> lastEdgeG, lastEdgeTranspose;
    vector<lli> piFromS, piFromT;
    
    void Dijkstra(int s, int t, vector<lli> &pi, const vector<Edge> &g, const vector<int> last);
public:
    int i;
    
    Traffic(int N, int M, int k, int s, int t);
    void CreateEdge(int u, int v, int w);
    void AddTwoWayRoad(int u, int v, int w);
    void FindMinLength(void);
};

Traffic::Traffic(int N, int M, int k, int s, int t) :
V(N), s(s), t(t), k(2*k), i(0),
twoWayRoads(2*k), Graph(M), transposeGraph(M), lastEdgeG(N, -1), lastEdgeTranspose(N, -1),
piFromS(N, infty), piFromT(N, infty)
{
}

void Traffic::CreateEdge(int u, int v, int w)
{
    Graph[i] = {u, v, w, lastEdgeG[u]};
    transposeGraph[i] = {v, u, w, lastEdgeTranspose[v]};
    
    lastEdgeG[u] = i;
    lastEdgeTranspose[v] = i++;
}

void Traffic::AddTwoWayRoad(int u, int v, int w)
{
    twoWayRoads[i++] = {u, v, w};
    twoWayRoads[i++] = {v, u, w};
}

void Traffic::Dijkstra(int s, int t, vector<lli> &pi, const vector<Edge> &g, const vector<int> last)
{
    priority_queue<Node, vector<Node>, greater<Node>> Q;
    vector<bool> visited(V, false);
    
    pi[s] = 0;
    Q.push({pi[s], s});
    
    while (!Q.empty())
    {
        int u = Q.top().second;
        Q.pop();
        
        if (visited[u])
            continue;
        
        if (u == t)
            break;
        
        visited[u] = true;
        
        for (int e = last[u]; e != -1; e = g[e].prev)
        {
            int v = g[e].v;
            
            if (!visited[v] && pi[v] > pi[u] + g[e].weight)
            {
                pi[v] = pi[u] + g[e].weight;
                Q.push({pi[v], v});
            }
        }
    }
}

void Traffic::FindMinLength(void)
{
    Dijkstra(s, t, piFromS, Graph, lastEdgeG);
    Dijkstra(t, s, piFromT, transposeGraph, lastEdgeTranspose);
    lli minLength = piFromS[t];
    
    if (piFromS[t] == infty)
        printf("%d\n", -1);
    else
    {
        for (int i = 0; i != k; ++i)
        {
            int u = twoWayRoads[i].u;
            int v = twoWayRoads[i].v;
            
            // check if u and v are in the path of the shortest path
            if (piFromS[u] != infty && piFromT[v] != infty)
                minLength = min(minLength, piFromS[u] + twoWayRoads[i].weight + piFromT[v]);
        }
        
        printf("%lld\n", minLength);
    }
}

int main(void)
{
    int nCases, n, m, k, s, t;
    
    scanf("%d", &nCases);
    
    while (nCases--)
    {
        scanf("%d %d %d %d %d", &n, &m, &k, &s, &t);
        
        --s; --t;
        Traffic game(n, m, k, s, t);
        
        for (int j = 0; j != m; ++j)
        {
            int u, v, w;
            scanf("%d %d %d", &u, &v, &w);
            --u; --v;
            game.CreateEdge(u, v, w);
        }
        
        game.i = 0;
        
        for (int j = 0; j != k; ++j)
        {
            int u, v, w;
            scanf("%d %d %d", &u, &v, &w);
            --u; --v;
            game.AddTwoWayRoad(u, v, w);
        }
        
        game.FindMinLength();
    }
    
    return 0;
}
