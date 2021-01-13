#include <vector>
#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>

using namespace std;

const int infty = 100 * 32767;

class DisjointPaths
{
private:
    int V, N, s, t, k;
    vector<vector<int>> Adj, cap;
    
    bool BFS(vector<int> &predecessor);
public:
    
    DisjointPaths(int k, int N);
    void CreateEdge(int u, int v, int c);
    void ConnectInOutVertex(int u, int v, int c);
    void GetDisjoinPaths(void);
    void PrintPaths(vector<int> &path, int u);
};

DisjointPaths::DisjointPaths(int k, int N) :
V(2*N), N(N), s(N), t(1), k(k), Adj(V), cap(V, vector<int>(V, 0))
{
}
void DisjointPaths::CreateEdge(int u, int v, int c)
{
    Adj[u].push_back(v+N);
    cap[u][v+N] = c;
}

void DisjointPaths::ConnectInOutVertex(int u, int v, int c)
{
    Adj[u].push_back(v);
    cap[u][v] = c;
}

bool DisjointPaths::BFS(vector<int> &predecessor)
{
    deque<int> Q;
    vector<bool> visited(V, false);
    
    visited[s] = true;
    Q.push_back(s);
    
    while (!Q.empty())
    {
        int u = Q.front();
        Q.pop_front();
        
        if (u == t)
            break;
        
        for (auto v : Adj[u])
            if (!visited[v] && cap[u][v] > 0)
            {
                visited[v] = true;
                predecessor[v] = u;
                Q.push_back(v);
            }
    }
    
    return predecessor[t] != -1;
}

void DisjointPaths::GetDisjoinPaths(void)
{
    vector<int> predecessor(V, -1);
    vector<vector<int>> paths;
    int kPaths = 0;
    
    cap[s][s-N] = infty;
    cap[t+N][t] = infty;
    
    while (BFS(predecessor) && kPaths != k)
    {
        ++kPaths;
        paths.push_back(predecessor);
        
        for (int v = t; v != s; v = predecessor[v])
        {
            int u = predecessor[v];
            --cap[u][v];
            ++cap[v][u];
        }
        
        fill(predecessor.begin(), predecessor.end(), -1);
    }
    
    if (kPaths == k)
    {
        int i = 0;
        
        while (i != k)
        {
            PrintPaths(paths[i++], t);
            printf("\n");
        }
    }
    else
        printf("Impossible\n");
}

void DisjointPaths::PrintPaths(vector<int> &predecessor, int u)
{
    if (u == -1)
        return ;
    
    PrintPaths(predecessor, predecessor[u]);
    if (u < N)
        printf("%d ", u+1);
}

int main(void)
{
    int k, N;
    
    int i = 1;
    while (scanf("%d", &k) == 1 && k != 0)
    {
        scanf("%d", &N);
        
        DisjointPaths game(k, N);
        
        string w;
        int u = 0;
        while (u != N && getline(cin, w))
        {
            if (w.empty())
                continue;
            
            stringstream ss(w);
            int v;
            
            while (ss >> v)
                game.CreateEdge(u, --v, 1);
            
            ++u;
        }
        
        for (int u = 0; u != N; ++u)
            game.ConnectInOutVertex(u+N, u, 1);
        
        printf("Case %d:\n", i);
        game.GetDisjoinPaths();
        printf("\n");
        ++i;
    }
    
    return 0;
}
