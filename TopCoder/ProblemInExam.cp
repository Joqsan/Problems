#include <vector>
#include <algorithm>
#include <cstdio>
#include <deque>

using namespace std;


class Graph
{
private:
    
    int V, nEdges;
    vector<vector<int>> Adj;
    vector<int> predecessor;
    vector<bool> InCycle;
    
    void DFS_Visit(int u, vector<bool> &visited);
public:
    Graph(int N);
    
    void CreateEdge(int u, int v);
    void FindCycle(void);
    
};

Graph::Graph(int N) :
V(N), Adj(N), predecessor(N, -1), nEdges(10000), InCycle(N, false)
{
}

void Graph::CreateEdge(int u, int v)
{
    Adj[u].push_back(v);
    Adj[v].push_back(u);
}

void Graph::FindCycle(void)
{
    vector<bool> visited(V, false);
    
    for (int u = 0; u != V; ++u)
        if (!visited[u])
            DFS_Visit(u, visited);
    
    printf("%d\n", nEdges);
}

void Graph::DFS_Visit(int u, vector<bool> &visited)
{
    visited[u] = true;
    
    for (auto v : Adj[u])
    {
        if (!visited[v])
        {
            visited[v] = true;
            predecessor[v] = u;
            DFS_Visit(v, visited);
        }
        else if (v != predecessor[u] && !InCycle[v])
        {
            int x = u;
            int counter = 0;
            while (x != -1)
            {
                ++counter;
                InCycle[x] = true;
                x = predecessor[x];
            }
            
            nEdges = min(nEdges, counter);
        }
    }
}

int main(void)
{
    FILE *fp = fopen("ProblemExam.txt", "r");
    
    int N, M;
    
    fscanf(fp, "%d %d", &N, &M);
    
    Graph game(N);
    
    for (int i = 0; i != M; ++i)
    {
        int u, v;
        fscanf(fp, "%d %d", &u, &v);
        --u; --v;
        game.CreateEdge(u, v);
    }
    
    game.FindCycle();
    
    return 0;
}
