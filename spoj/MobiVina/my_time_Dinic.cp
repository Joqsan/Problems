#include <vector>
#include <algorithm>
#include <deque>
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
    vector<int> lastEdge, levels;
    
    int PushFlow(int u, int FlowReachingU);
    bool BFS(void);
    
public:
    
    MobiVina(int N);
    void CreateEdge(int u, int v, int cap);
    void FindMinCut(void);
};

MobiVina::MobiVina(int N) :
V(N+2), s(0), t(V-1), i(0), Residual(2*(2*N + N*N)), lastEdge(V, -1), levels(V)
{
}

void MobiVina::CreateEdge(int u, int v, int cap)
{
    Residual[i] = {u, v, cap, lastEdge[u]};
    lastEdge[u] = i++;
    
    Residual[i] = {v, u, 0, lastEdge[v]};
    lastEdge[v] = i++;
}


int MobiVina::PushFlow(int u, int flowReachingU)
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


bool MobiVina::BFS(void)
{
    deque<int> Q;
    vector<bool> visited(V, false);
    
    visited[s] = true;
    levels[s] = 0;
    Q.push_back(s);
    
    while (!Q.empty())
    {
        int u = Q.front();
        Q.pop_front();
        
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

void MobiVina::FindMinCut(void)
{
    int maxFlow = 0;
    
    while (BFS())
        maxFlow += PushFlow(s, infty);
    
    
    printf("%d\n", maxFlow);
}


int main(void)
{
    
    int totalClocks = 0;
    clock_t start, end;
    
    for (int k = 0; k != 100;++k)
    {
        FILE *fp = fopen("mobi.txt", "r");
        
        int N;
        
        start = clock();
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
        
        game.FindMinCut();
        
        end = clock();
        
        totalClocks += end - start;
    }
    
    printf("%d\n", totalClocks/100);
    
    return 0;
}
