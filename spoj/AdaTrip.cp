#include <vector>
#include <set>
#include <queue>
#include <utility>
#include <cstdio>

using namespace std;

const int infty = int(1e9);

typedef pair<int, int> Node;

class AdaTrip
{
private:
    int V;
    vector<set<int>> Adj;
    vector<vector<int>> weight;
public:
    
    AdaTrip(int N);
    void CreateEdge(int u, int v, int w);
    void Dijkstra(int s);
};

AdaTrip::AdaTrip(int N) :
V(N), Adj(N), weight(N, vector<int>(N, infty))
{
}

void AdaTrip::CreateEdge(int u, int v, int w)
{
    if (Adj[u].insert(v).second)
    {
        Adj[v].insert(u);
        weight[u][v] = weight[v][u] = w;
    }
    else
    {
        weight[u][v] = min(weight[u][v], w);
        weight[v][u] = weight[u][v];
    }
}

void AdaTrip::Dijkstra(int s)
{
    priority_queue<Node, vector<Node>, greater<Node>> Q;
    vector<bool> visited(V, false);
    vector<int> pi(V, infty);
    int max = 0;
    int counter = 0;
    
    pi[s] = 0;
    Q.push({pi[s], s});
    
    while (!Q.empty())
    {
        int u = Q.top().second; Q.pop();
        
        if (visited[u])
            continue;
        
        visited[u] = true;
        
        if (pi[u] > max)
        {
            max = pi[u];
            counter = 1;
        }
        else if (pi[u] == max)
            ++counter;
        
        for (auto v : Adj[u])
            if (!visited[v] && pi[v] > pi[u] + weight[u][v])
            {
                pi[v] = pi[u] + weight[u][v];
                Q.push({pi[v], v});
            }
    }
    
    printf("%d %d\n", max, counter);
    
}

int main(void)
{
    int N, M, Q;
    
    scanf("%d %d %d", &N, &M, &Q);
    
    AdaTrip game(N);
    
    for (int i = 0; i != M; ++i)
    {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        
        if (u != v)
            game.CreateEdge(u, v, w);
    }
    
    for (int i = 0; i != Q; ++i)
    {
        int s;
        scanf("%d", &s);
        game.Dijkstra(s);
    }
    
    return 0;
    
}
