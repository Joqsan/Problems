#include <vector>
#include <algorithm>
#include <deque>
#include <cstdio>

using namespace std;

struct Edge
{
    int u, v, cap, prev;
};


const int infty = 100 * 32767;

class MMatch
{
private:
    int V, i;
    vector<Edge> Residual;
    vector<int> lastEdge, levels;
    
    bool BFS(void);
    int PushFlow(const int &u, int flowReachingU);
public:
    int s, t;
    
    MMatch(int L, int R);
    void CreateGraph(int u, int v, int cap);
    void FindHappyKids(void);
    
};

MMatch::MMatch(int L, int R) :
V(L+R+2), s(0), t(V-1), i(0), Residual(2*(L*R + L + R)), lastEdge(L+R+2, -1), levels(L+R+2)
{
}

void MMatch::CreateGraph(int u, int v, int cap)
{
    Residual[i] = {u, v, cap, lastEdge[u]};
    lastEdge[u] = i++;
    
    Residual[i] = {v, u, 0, lastEdge[v]};
    lastEdge[v] = i++;
}

int MMatch::PushFlow(const int &u, int flowReachingU)
{
    int flowLeavingU = 0;
    
    if (u == t)
        return flowReachingU;
    
    for (int e = lastEdge[u]; e != -1; e = Residual[e].prev)
    {
        int v = Residual[e].v;
        
        if (flowReachingU > 0)
        {
            if (Residual[e].cap > 0 && levels[v] == levels[u] + 1)
            {
                int c_fp = min(flowReachingU, Residual[e].cap);
                
                int flowPushed = PushFlow(v, c_fp);
                
                if (flowPushed != 0)
                {
                    --Residual[e].cap;
                    ++Residual[e^1].cap;
                    
                    --flowReachingU;
                    ++flowLeavingU;
                }
            }
        }
        else
            break;
    }
    
    return flowLeavingU;
}

bool MMatch::BFS(void)
{
    deque<int> Q;
    vector<bool> visited(V, false);
    
    levels[s] = 0;
    visited[s] = true;
    Q.push_back(s);
    
    while (!Q.empty())
    {
        int u = Q.front();
        Q.pop_front();
        
        for (int e = lastEdge[u]; e != -1; e = Residual[e].prev)
        {
            int v = Residual[e].v;
            
            if (!visited[v] && Residual[e].cap > 0)
            {
                visited[v] = true;
                levels[v] = levels[u] + 1;
                Q.push_back(v);
            }
        }
    }
    
    return visited[t];
}

void MMatch::FindHappyKids(void)
{
    int maxFlow = 0;
    while (BFS())
        maxFlow += PushFlow(s, infty);
    
    printf("%d\n", maxFlow);
}

int main(void)
{
    int nCases, L, R;
    
    scanf("%d", &nCases);
    
    while (nCases--)
    {
        scanf("%d %d", &L, &R);
        
        MMatch game(L, R);
        
        int u, v;
        while (scanf("%d %d", &u, &v) == 2 && u != 0)
            game.CreateGraph(u, L+v, 1);
        
        for (int u = 1; u != L+1; ++u)
            game.CreateGraph(game.s, u, 1);
        
        for (int u = 1; u != R+1; ++u)
            game.CreateGraph(L+u, game.t, 1);
        
        game.FindHappyKids();
    }
    
    return 0;
}
