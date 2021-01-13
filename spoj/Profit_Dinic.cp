#include <vector>
#include <deque>
#include <algorithm>
#include <cstdio>


using namespace std;

const int infty = int(1e9);

struct Edge
{
    int u, v, cap, prev;
};

class Profit
{
private:
    int V, i, s, t;
    vector<Edge> Residual;
    vector<int> lastEdge, levels;
    
    int PushFlow(int u, int flowReachingU);
    bool BFS(void);
public:
    
    Profit(int N, int M);
    void CreateEdge(int u, int v, int cap);
    void FindMaxRevenue(const int &totalProfit);
};

Profit::Profit(int N, int M) :
V(N + M + 2), i(0), s(0), t(V-1), Residual(2*(2*M + N + M)), lastEdge(V, -1), levels(V)
{
}

void Profit::CreateEdge(int u, int v, int cap)
{
    Residual[i] = {u, v, cap, lastEdge[u]};
    lastEdge[u] = i++;
    
    Residual[i] = {v, u, 0, lastEdge[v]};
    lastEdge[v] = i++;
}

bool Profit::BFS(void)
{
    deque<int> Q;
    vector<bool> visited(V, false);
    
    levels[s] = 0;
    visited[s] = true;
    Q.push_back(s);
    
    while (!visited[t] && !Q.empty())
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

int Profit::PushFlow(int u, int flowReachingU)
{
    int flowLeavingU = 0;
    
    if (u == t)
        return flowReachingU;
    
    for (int e = lastEdge[u]; e != -1 && flowReachingU > 0; e = Residual[e].prev)
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
    
    return flowLeavingU;
}


void Profit::FindMaxRevenue(const int &totalProfit)
{
    int maxFlow = 0;
    while (BFS())
        maxFlow += PushFlow(s, infty);
    
    printf("%d\n", totalProfit - maxFlow);
}

int main(void)
{
    int nCases;
    
    scanf("%d", &nCases);
    
    while (nCases--)
    {
        int N, M;
        
        scanf("%d %d", &N, &M);
        
        Profit game(N, M);
        
        for (int u = 1; u != N+1; ++u)
        {
            int cost;
            scanf("%d", &cost);
            game.CreateEdge(0, u, cost);
        }
        
        int t = N + M + 1;
        int totalProfit = 0;
        vector<bool> ConnectedToSink(M, false);
        for (int v = 1; v != M+1; ++v)
        {
            int u1, u2, profit;
            scanf("%d %d %d", &u1, &u2, &profit);
            
            totalProfit += profit;
            
            game.CreateEdge(u1, v+N, infty);
            game.CreateEdge(u2, v+N, infty);
            
            if (!ConnectedToSink[v-1])
            {
                game.CreateEdge(v+N, t, profit);
                ConnectedToSink[v-1] = true;
            }
        }
        
        game.FindMaxRevenue(totalProfit);
    }
    
    return 0;
}
