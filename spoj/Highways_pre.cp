#include <vector>
#include <utility>
#include <queue>
#include <cstdio>

using namespace std;

typedef pair<int, int> Node;

struct Edge
{
    int u, v, weight, prev;
};

const int infty = 100 * 32767;

class HighWays
{
private:
    
    vector<Edge> Graph;
    vector<int> lastEdge;
    
public:
    int V, i;
    
    HighWays(int N, int M);
    void CreateEdge(int u, int v, int w);
    void FindMinTime(const int &s, const int &t);
};

HighWays::HighWays(int N, int M) :
V(N), i(0), Graph(2*M), lastEdge(N, -1)
{
}

void HighWays::CreateEdge(int u, int v, int w)
{
    Graph[i] = {u, v, w, lastEdge[u]};
    lastEdge[u] = i++;
    
    Graph[i] = {v, u, w, lastEdge[v]};
    lastEdge[v] = i++;
}

void HighWays::FindMinTime(const int &s, const int &t)
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
        
        if (u == t)
            break;
        
        if (visited[u])
            continue;
        
        visited[u] = true;
        
        for (int e = lastEdge[u]; e != -1; e = Graph[e].prev)
        {
            int v = Graph[e].v;
            
            if (!visited[v] && pi[v] > pi[u] + Graph[e].weight)
            {
                pi[v] = pi[u] + Graph[e].weight;
                Q.push({pi[v], v});
            }
        }
    }
    
    if (pi[t] == infty)
        printf("NONE\n");
    else
        printf("%d\n", pi[t]);
}

int main(void)
{
    FILE *fp = fopen("highways.txt", "r");
    int nCases, n, m, s, t;
    
    fscanf(fp, "%d", &nCases);
    
    while (nCases--)
    {
        fscanf(fp, "%d %d %d %d", &n, &m, &s, &t);
        --s; --t;
        
        HighWays game(n, m);
        
        for (int i = 0; i != m; ++i)
        {
            int u, v, w;
            fscanf(fp, "%d %d %d", &u, &v, &w);
            --u; --v;
            game.CreateEdge(u, v, w);
        }
        
        game.FindMinTime(s, t);
    }
    
    return 0;
    
}
