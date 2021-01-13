#include <vector>
#include <algorithm>
#include <deque>
#include <cstdio>

using namespace std;

struct Edge
{
    int u, v, cap, prev;
};

class SellPhone2
{
private:
    int V, L, i, superSource;
    vector<Edge> Residual;
    vector<int> lastEdge, levels;
    
    int PushFlow(int u, int FlowReachingU);
    bool BFS(void);
    
public:
    int s, t;
    SellPhone2(int N, int M, int L);
    void CreateEdge(int u, int v, int cap);
    void FindPhoneSold(void);
};

SellPhone2::SellPhone2(int N, int M, int L) :
V(N+M+2+1), L(L), s(0), t(V-2), i(0), Residual(2*(M*N + M + N + 1)), lastEdge(V, -1), levels(V),
superSource(V-1)
{
}

void SellPhone2::CreateEdge(int u, int v, int cap)
{
    Residual[i] = {u, v, cap, lastEdge[u]};
    lastEdge[u] = i++;
    
    Residual[i] = {v, u, 0, lastEdge[v]};
    lastEdge[v] = i++;
}


int SellPhone2::PushFlow(int u, int flowReachingU)
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


bool SellPhone2::BFS(void)
{
    deque<int> Q;
    vector<bool> visited(V, false);
    
    visited[superSource] = true;
    levels[superSource] = 0;
    Q.push_back(superSource);
    
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

void SellPhone2::FindPhoneSold(void)
{
    int maxPhone = 0;
    
    CreateEdge(superSource, s, L);
    
    while (BFS())
        maxPhone += PushFlow(superSource, L);
    
    
    printf("%d\n", maxPhone);
}


int main(void)
{
    FILE *fp = fopen("sellphone.txt", "r");
    int nCases, L, M, N;
    
    fscanf(fp, "%d", &nCases);
    
    while (nCases--)
    {
        fscanf(fp, "%d %d %d", &L, &M, &N);
        
        SellPhone2 game(M, N, L);
        
        for (int u = 1; u != M+1; ++u)
        {
            int c;
            fscanf(fp, "%d", &c);
            game.CreateEdge(game.s, u, c);
        }
        
        for (int v = 1; v != N+1; ++v)
        {
            int c;
            fscanf(fp, "%d", &c);
            game.CreateEdge(v+M, game.t, c);
        }
        
        for (int u = 1; u != M+1; ++u)
            for (int v = 1; v != N+1; ++v)
            {
                int c;
                fscanf(fp, "%d", &c);
                
                if (c != 0)
                    game.CreateEdge(u, v+M, c);
            }
        
        game.FindPhoneSold();
    }
    
    return 0;
}
