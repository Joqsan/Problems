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

class Coconuts
{
private:
    int V, s, t, i;
    vector<Edge> Residual;
    vector<int> lastEdge, levels;
    
    bool BFS(void);
    int PushFlow(int u, int flowReachingU);
public:
    
    Coconuts(int N, int M);
    
    void CreateEdge(int u, int v, int cap);
    void ConnectToSourceSink(int u, int v, int cap);
    void FindDisagreements(void);
    
    
};

Coconuts::Coconuts(int N, int M) :
V(N+2), s(0), t(V-1), i(0), Residual(2*(2*M + N)), lastEdge(V, -1), levels(V)
{
}

void Coconuts::CreateEdge(int u, int v, int cap)
{
    Residual[i] = {u, v, cap, lastEdge[u]};
    lastEdge[u] = i++;
    
    Residual[i] = {v, u, cap, lastEdge[v]};
    lastEdge[v] = i++;
}

void Coconuts::ConnectToSourceSink(int u, int v, int cap)
{
    Residual[i] = {u, v, cap, lastEdge[u]};
    lastEdge[u] = i++;
    
    Residual[i] = {v, u, 0, lastEdge[v]};
    lastEdge[v] = i++;
}

bool Coconuts::BFS(void)
{
    deque<int> Q;
    vector<bool> visited(V, false);
    
    visited[s] = true;
    levels[s] = 0;
    Q.push_back(s);
    
    while (!Q.empty())
    {
        int u = Q.front(); Q.pop_front();
        
        for (int e = lastEdge[u]; e != -1; e = Residual[e].prev)
        {
            int v = Residual[e].v;
            
            if (Residual[e].cap > 0 && !visited[v])
            {
                visited[v] = true;
                levels[v] = levels[u] + 1;
                Q.push_back(v);
            }
        }
    }
    
    return visited[t];
}

int Coconuts::PushFlow(int u, int flowReachingU)
{
    int flowLeavingU = 0;
    
    if (u == t)
        return flowReachingU;
    
    for (int e = lastEdge[u]; e != -1; e = Residual[e].prev)
    {
        if (flowReachingU > 0)
        {
            int v = Residual[e].v;
            
            if (Residual[e].cap > 0 && levels[v] == levels[u] + 1)
            {
                int c_fp = min(flowReachingU, Residual[e].cap);
                
                int flowPushed = PushFlow(v, c_fp);
                
                if (flowPushed > 0)
                {
                    Residual[e].cap -= flowPushed;
                    Residual[e^1].cap += flowPushed;
                    
                    flowReachingU -= flowPushed;
                    flowLeavingU += flowPushed;
                }
            }
        }
        else
            break;
    }
    
    return flowLeavingU;
}

void Coconuts::FindDisagreements(void)
{
    int maxFlow = 0;
    
    while (BFS())
        maxFlow += PushFlow(s, infty);
    
    printf("%d\n", maxFlow);
}

int main(void)
{
    FILE *fp = fopen("coconuts.txt", "r");
    
    int N, M;
    
    while (fscanf(fp, "%d %d", &N, &M) == 2 && N != 0)
    {
        Coconuts game(N, M);
        
        int t = N + 1;
        for (int u = 1; u != N+1; ++u)
        {
            int x;
            fscanf(fp, "%d", &x);
            
            if (x == 1)
                game.ConnectToSourceSink(0, u, 1);
            else
                game.ConnectToSourceSink(u, t, 1);
        }
        
        for (int i = 0; i != M; ++i)
        {
            int u, v;
            fscanf(fp, "%d %d", &u, &v);
            game.CreateEdge(u, v, 1);
        }
        
        game.FindDisagreements();
    }
    return 0;
    
}
