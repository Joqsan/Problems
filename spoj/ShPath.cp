#include <vector>
#include <queue>
#include <utility>
#include <cstdio>
#include <unordered_map>
#include <string>
#include <set>
#include <iostream>

using namespace std;

typedef pair<int, int> Node;

const int infty = 100 * 32767;


class SHPATH
{
private:
    int V;
    vector<vector<int>> Adj;
    vector<vector<int>> weight;
    
public:
    SHPATH(int N);
    void CreateEdge(int u, int v, int w);
    
    void Dijkstra(int s, int t);
};

SHPATH::SHPATH(int N) :
V(N), Adj(N), weight(N, vector<int>(N, 0))
{
}

void SHPATH::CreateEdge(int u, int v, int w)
{
    Adj[u].push_back(v);
    weight[u][v] = w;
}

void SHPATH::Dijkstra(int s, int t)
{
    priority_queue<Node, vector<Node>, greater<Node>> Q;
    vector<int> pi(V, infty);
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
        
        for (auto v : Adj[u])
            if (pi[v] > pi[u] + weight[u][v])
            {
                pi[v] = pi[u] + weight[u][v];
                Q.push({pi[v], v});
            }
    }
    
    printf("%d\n", pi[t]);
}

int main(void)
{
    unordered_map<string, int> numberedCities;
    
    int nCases;
    
    
    scanf("%d", &nCases);
    
    while (nCases--)
    {
        int nCities;
        scanf("%d", &nCities);
        
        SHPATH game(nCities);
        
        for (int u = 0; u != nCities; ++u)
        {
            string city;
            cin >> city;
            numberedCities[city] = u;
            
            int n;
            
            scanf("%d", &n);
            
            while (n--)
            {
                int v, w;
                scanf("%d %d", &v, &w);
                --v;
                game.CreateEdge(u, v, w);
            }
        }
        
        int nPaths;
        
        scanf("%d", &nPaths);
        
        while (nPaths--)
        {
            string source, dest;
            
            cin >> source >> dest;
            game.Dijkstra(numberedCities[source], numberedCities[dest]);
        }
    }
    return 0;
}
