#include <vector>
#include <deque>
#include <cstdio>

using namespace std;

struct Edge
{
    int u, v, cap, prev;
};

const int infty = int(1e9);

class CableTV
{
private:
    int V, i;
    vector<Edge> Residual;
    vector<int> lastEdge;
    
public:
    
    CableTV(int N, int M);
    bool BFS(const int &s, const int &t);
    void CreateEdge(int u, int v, int cap);
};

CableTV::CableTV(int N, int M) :
V(2*N), i(0), Residual(2*(2*M + N)), lastEdge(V, -1)
{
}

void CableTV::CreateEdge(int u, int v, int cap)
{
    Residual[i] = {u, v, cap, lastEdge[u]};
    lastEdge[u] = i++;
    
    Residual[i] = {v, u, 0, lastEdge[v]};
    lastEdge[v] = i++;
}

bool CableTV::BFS(const int &s, const int &t)
{
    deque<int> Q;
    vector<bool> visited(V, false);
    vector<int> inEdge(V, -1);
    
    visited[s] = true;
    Q.push_back(s);
    
    while (!visited[t] && !Q.empty())
    {
        int u = Q.front(); Q.pop_front();
        
        for (int e = lastEdge[u]; e != -1 && !visited[t]; e = Residual[e].prev)
        {
            int v = Residual[e].v;
            
            if (Residual[e].cap > 0 && !visited[v])
            {
                visited[v] = true;
                inEdge[v] = e;
                Q.push_back(v);
            }
        }
    }
    
    
    if (visited[t])
    {
        for (int v = t; v != s; v = Residual[inEdge[v]].u)
        {
            --Residual[inEdge[v]].cap;
            ++Residual[inEdge[v]^1].cap;
        }
        
        return true;
    }
    else
        return false;
}

int main(void)
{
    int nCases;
    
    scanf("%d", &nCases);
    
    while (nCases--)
    {
        int N, M;
        
        scanf("%d %d", &N, &M);
        
        if (M == 0)
        {
            if (N == 1)
                printf("%d\n", 1);
            else
                printf("%d\n", 0);
            
            continue;
        }
        
        CableTV game(N, M);
        
        char edge[8]; // (00,00)\n
        for (int i = 0; i != M; ++i)
        {
            int u, v;
            scanf("%s", edge);
            sscanf(edge, "(%d,%d)", &u, &v);
            game.CreateEdge(u, v+N, infty);
            game.CreateEdge(v, u+N, infty);
        }
        
        for (int u = 0; u != N; ++u)
            game.CreateEdge(u+N, u, 1);
        
        int counter = 0;
        for (int t = 1; t != N; ++t)
        {
            if (game.BFS(0, t))
                ++counter;
            else
                break;
        }
        
        if (counter == N-1)
            printf("%d\n", N);
        else
            printf("%d\n", counter);
    }
    return 0;
}
