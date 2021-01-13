#include <vector>
#include <deque>
#include <algorithm>
#include <cstdio>

using namespace std;

const int infty = int(1e6);

struct Edge
{
    int u, v, cap, prev;
};

class Quest4
{
private:
    int V, s, t, i;
    vector<Edge> Residual;
    vector<int> lastEdge, levels;
    
    bool BFS(void);
    int PushFlow(int u, int flowReachingU);
public:
    
    Quest4(int M);
    void CreateEdge(int u, int v, int cap);
    void FindMinPlanks(void);
    
};

Quest4::Quest4(int M) :
V(122*122), s(V-2), t(V-1), i(0),
Residual(6*M), lastEdge(V, -1), levels(V)
{
}

void Quest4::CreateEdge(int u, int v, int cap)
{
    Residual[i] = {u, v, cap, lastEdge[u]};
    lastEdge[u] = i++;
    
    Residual[i] = {v, u, 0, lastEdge[v]};
    lastEdge[v] = i++;
}

bool Quest4::BFS(void)
{
    deque<int> Q;
    vector<bool> visited(V, false);
    
    visited[s] = true;
    levels[s] = 0;
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

int Quest4::PushFlow(int u, int flowReachingU)
{
    int flowLeavingU = 0;
    
    if (u == t)
        return flowReachingU;
    
    for (int e = lastEdge[u]; e != -1 && flowReachingU; e = Residual[e].prev)
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

void Quest4::FindMinPlanks(void)
{
    int maxFlow = 0;
    
    while (BFS())
        maxFlow += PushFlow(s, infty);
    
    printf("%d\n", maxFlow);
}

int main(void)
{
    FILE *fp = fopen("quest4.txt", "r");
    
    int nRooms, M;
    
    fscanf(fp, "%d", &nRooms);
    
    while (nRooms--)
    {
    
        fscanf(fp, "%d", &M);
        
        Quest4 game(M);
        
        vector<bool> cells(122*122);
        
        for (int i = 0; i != M; ++i)
        {
            int u, v;
            fscanf(fp, "%d %d", &u, &v);
            v += 120;
            
            if (!cells[u])
            {
                game.CreateEdge(122*122-2, u, 1);
                cells[u] = true;
            }
            
            if (!cells[v])
            {
                game.CreateEdge(v, 122*122-1, 1);
                cells[v] = true;
            }
            
            game.CreateEdge(u, v, 1);
        }
        
        game.FindMinPlanks();
    }
    
    return 0;
}
