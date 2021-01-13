#include <vector>
#include <algorithm>
#include <cstdio>

using namespace std;

const int infty = 100 * 32767;

struct Edge
{
    int u, v, cap, prev;
};

class Matching
{
private:
    int V, i;
    vector<Edge> Residual;
    vector<int> lastEdge, levels;
    bool BFS(void);
    int PushFlow(const int &u, int flowReachingU);
public:
    int s, t;
    Matching(int L, int R, int p);
    void CreateEdge(int u, int v, int cap);
    void MaxPair(void);
};

Matching::Matching(int L, int R, int p) :
V(L + R + 2), i(0), s(0), t(V-1), Residual(2*(p + L + R)), lastEdge(V, -1), levels(V)
{
}

void Matching::CreateEdge(int u, int v, int cap)
{
    Residual[i] = {u, v, cap, lastEdge[u]};
    lastEdge[u] = i++;
    
    Residual[i] = {v, u, 0, lastEdge[v]};
    lastEdge[v] = i++;
}

bool Matching::BFS(void)
{
    vector<int> Q(V);
    int qh(0), qt(0);
    vector<bool> visited(V, false);
    
    levels[s] = 0; visited[s] = true;
    
    Q[qt++] = s;
    while (qh != qt)
    {
        int u = Q[qh++];
        
        if (u == t)
            break;
        
        for (int e = lastEdge[u]; e != -1; e = Residual[e].prev)
        {
            int v = Residual[e].v;
            
            if (visited[v] == false && Residual[e].cap > 0)
            {
                visited[v] = true;
                levels[v] = levels[u] + 1;
                Q[qt++] = v;
            }
        }
    }
    
    return visited[t];
}

int Matching::PushFlow(const int &u, int flowReachingU)
{
    int netFlowLeavingU = 0;
    
    if (u == t)
        return flowReachingU;
    
    for (int e = lastEdge[u]; e != -1; e = Residual[e].prev)
    {
        if (flowReachingU > 0)
        {
            int v = Residual[e].v;
            if (levels[v] == levels[u] + 1 && Residual[e].cap > 0)
            {
                int flowPushed = min(flowReachingU, Residual[e].cap);
                
                int total_cfp = PushFlow(v, flowPushed);
                
                if (total_cfp)
                {
                    flowReachingU -= total_cfp;
                    netFlowLeavingU += total_cfp;
                    
                    Residual[e].cap -= total_cfp;
                    Residual[e^1].cap += total_cfp;
                }
            }
        }
        else
            break;
    }
    
    return netFlowLeavingU;
}

void Matching::MaxPair(void)
{
    int maxFlow = 0;
    
    while (BFS())
        maxFlow += PushFlow(s, infty);
    
    printf("%d\n", maxFlow);
}

int main(void)
{
    int L, R, p;
    
    scanf("%d %d %d", &L, &R, &p);
    
    Matching game(L, R, p);
    
    for (int i = 0; i != p; ++i)
    {
        int u, v;
        scanf("%d %d", &u, &v);
        game.CreateEdge(u, v+L, 1);
    }
    
    for (int i = 1; i != L+1; ++i)
        game.CreateEdge(game.s, i, 1);
    for (int i = 1; i != R+1; ++i)
        game.CreateEdge(i+L, game.t, 1);
    
    game.MaxPair();
    
    return 0;
    
}

