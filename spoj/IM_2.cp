#include <vector>
#include <deque>
#include <cstdio>
#include <algorithm>

using namespace std;

bool AreInRange(int N, int u, int v)
{
    return u >= 0 && u < N && v >= 0 && v < N;
}

struct Edge
{
    int u, v, cap, prev;
};


class IGMap
{
private:
    int V, i, t;
    vector<Edge> Residual;
    vector<int> lastEdge;
    
    bool BFS(int source, vector<int> &inEdge);
public:
    
    IGMap(int N, int M);
    void CreateEdge(int u, int v, int cap);
    void FindValidPath(void);
};

IGMap::IGMap(int N, int M) :
V(N), i(0), Residual(2*M), lastEdge(N, -1)
{
}

void IGMap::CreateEdge(int u, int v, int cap)
{
    Residual[i] = {u, v, cap, lastEdge[u]};
    lastEdge[u] = i++;
    
    Residual[i] = {v, u, cap, lastEdge[v]};
    lastEdge[v] = i++;
}

bool IGMap::BFS(int source, vector<int> &inEdge)
{
    deque<int> Q;
    vector<bool> visited(V, false);
    
    visited[source] = true;
    Q.push_back(source);
    
    while (!visited[t] && !Q.empty())
    {
        int u = Q.front();
        Q.pop_front();
        
        for (int e = lastEdge[u]; e != -1; e = Residual[e].prev)
        {
            int v = Residual[e].v;
            
            if (Residual[e].cap > 0 && !visited[v])
            {
                visited[v] = true;
                inEdge[v] = e;
                
                if (v == t)
                    break;
                
                Q.push_back(v);
            }
        }
    }
    
    return visited[t];
}

void IGMap::FindValidPath(void)
{
    // 0: Naboo; 1: Tatooine; 2: Coruscant
    t = 1;
    vector<int> inEdge(V, -1);
    
    if (BFS(0, inEdge))
    {
        for (int v = t; v != 0; v = Residual[inEdge[v]].u)
        {
            --Residual[inEdge[v]].cap;
            ++Residual[inEdge[v]^1].cap;
        }
        
        if (BFS(2, inEdge))
            printf("YES\n");
        else
            printf("NO\n");
    }
    else
        printf("NO\n");
}

int main(void)
{
    int nCases, n, m;
    
    scanf("%d", &nCases);
    
    while (nCases--)
    {
        scanf("%d %d", &n, &m);
        
        IGMap game(n, m);
        
        for (int i = 0; i != m; ++i)
        {
            int u, v;
            scanf("%d %d", &u, &v);
            --u; --v;
            
            if (AreInRange(n, u, v))
                game.CreateEdge(u, v, 1);
        }
        
        game.FindValidPath();
    }
}

