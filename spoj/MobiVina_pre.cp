#include <vector>
#include <deque>
#include <algorithm>
#include <cstdio>

using namespace std;

struct Edge
{
    int u, v, cap, prev;
};

const int infty = int(1e9);

class MobiVina
{
private:
    int V, s, t, i;
    
    vector<Edge> Residual;
    vector<int> lastEdge;
    
    
    void Reflow(int flow, const vector<int> &inEdge);
public:
    int maxFlow;
    MobiVina(int N);
    void CreateEdge(int u, int v, int cap);
    bool BFS(void);
};

MobiVina::MobiVina(int N) :
V(N + 2), s(0), t(V-1), i(0), Residual(2*(N + N*N + N)), lastEdge(V, -1), maxFlow(0)
{
}

void MobiVina::CreateEdge(int u, int v, int cap)
{
    Residual[i] = {u, v, cap, lastEdge[u]};
    lastEdge[u] = i++;
    
    Residual[i] = {v, u, 0, lastEdge[v]};
    lastEdge[v] = i++;
}


bool MobiVina::BFS(void)
{
    deque<int> Q;
    vector<int> flows(V, 0), inEdge(V, -1);
    
    flows[s] = infty;
    Q.push_back(s);
    
    while (flows[t] == 0 && !Q.empty())
    {
        int u = Q.front(); Q.pop_front();
        
        for (int e = lastEdge[u]; e != -1; e = Residual[e].prev)
        {
            int v = Residual[e].v;
            
            if (Residual[e].cap > 0 && flows[v] == 0)
            {
                flows[v] = min(flows[u], Residual[e].cap);
                inEdge[v] = e;
                Q.push_back(v);
            }
        }
    }
    
    
    if (flows[t] > 0)
    {
        Reflow(flows[t], inEdge);
        return true;
    }
    else
        return false;
    
}

void MobiVina::Reflow(int flow, const vector<int> &inEdge)
{
    maxFlow += flow;
    
    for (int v = t; v != s; v = Residual[inEdge[v]].u)
    {
        Residual[inEdge[v]].cap -= flow;
        Residual[inEdge[v]^1].cap += flow;
    }
}

int main(void)
{
    FILE *fp = fopen("mobi.txt", "r");
    
    int N;
    
    fscanf(fp, "%d", &N);
    
    MobiVina game(N);
    
    for (int u = 1; u != N+1; ++u)
    {
        int cap;
        fscanf(fp, "%d", &cap);
        game.CreateEdge(0, u, cap);
    }
    
    int t = N + 1;
    
    for (int v = 1; v != N+1; ++v)
    {
        int cap;
        fscanf(fp, "%d", &cap);
        game.CreateEdge(v, t, cap);
    }
    
    for (int u = 1; u != N+1; ++u)
        for (int v = 1; v != N+1; ++v)
            {
                int cap;
                fscanf(fp, "%d", &cap);
                
                if (cap != 0)
                    game.CreateEdge(u, v, cap);
            }
    
    while (game.BFS());
    
    printf("%d\n", game.maxFlow);
    
    return 0;
}
