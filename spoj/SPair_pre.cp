#include <vector>
#include <deque>
#include <utility>
#include <algorithm>
#include <cstdio>

using namespace std;

typedef pair<int, int> Node;
const int infty = 100 * 32767;

struct Edge
{
    int u, v, cap, cost, prev;
};

class SPair
{
private:
    int V, s, t, i;
    
    vector<Edge> Residual;
    vector<int> lastEdge;
    vector<vector<int>> Adj;
    
    bool Levit(vector<int> &inEdge, vector<int> &pi);
public:
    
    SPair(int N, int M, int s, int t);
    
    void CreateEdge(int u, int v, int capacity, int edgeCost);
    void FindPairPath(void);
};

SPair::SPair(int N, int M, int s, int t) :
V(2*N-2), s(s), t(t), i(0), Residual(2*(M + N-2)), lastEdge(V, -1), Adj(V, vector<int>(V, -1))
{
}

void SPair::CreateEdge(int u, int v, int capacity, int edgeCost)
{
    int &edge = Adj[u][v];
    if (edge == -1 || (u == s && v == t))
    {
        Residual[i] = {u, v, capacity, edgeCost, lastEdge[u]};
        edge = i;
        lastEdge[u] = i++;
        
        Residual[i] = {v, u, 0, -edgeCost, lastEdge[v]};
        Adj[v][u] = i;
        lastEdge[v] = i++;
    }
    else
    {
        Residual[edge].cost = min(Residual[edge].cost, edgeCost);
        Residual[edge^1].cost = -Residual[edge].cost;
    }
}

bool SPair::Levit(vector<int> &inEdge, vector<int> &pi)
{
    deque<int> Q;
    vector<int> id(V, 0);
    
    pi[s] = 0;
    Q.push_back(s);
    
    while (!Q.empty())
    {
        int u = Q.front();
        Q.pop_front();
        
        id[u] = 2;
        
        for (int e = lastEdge[u]; e != -1; e = Residual[e].prev)
        {
            int v = Residual[e].v;
            
            if (Residual[e].cap > 0 && pi[v] > pi[u] + Residual[e].cost)
            {
                pi[v] = pi[u] + Residual[e].cost;
                inEdge[v] = e;
                
                if (id[v] == 2)
                    Q.push_front(v);
                else if (id[v] == 0)
                    Q.push_back(v);
                
                id[v] = 1;
                
            }
            
            
        }
    }
    
    return inEdge[t] != -1;
}

void SPair::FindPairPath(void)
{
    int nPaths = 0;
    int minCost = 0;
    vector<int> inEdge(V, -1), pi(V, infty);
    
    while (Levit(inEdge, pi))
    {
        ++nPaths;
        minCost += pi[t];
        
        for (int v = t; v != s; v = Residual[inEdge[v]].u)
        {
            --Residual[inEdge[v]].cap;
            ++Residual[inEdge[v]^1].cap;
        }
        
        if (nPaths != 2)
        {
            fill(inEdge.begin(), inEdge.end(), -1);
            fill(pi.begin(), pi.end(), infty);
        }
        else
            break;
    }
    
    if (nPaths != 2)
        printf("Not possible\n");
    else
        printf("%d\n", minCost);
}

int main(void)
{
    FILE *fp = fopen("spair.txt", "r");
    int n, m, nInstance;
    
    nInstance = 0;
    while (fscanf(fp, "%d %d", &n, &m) == 2 && n != 0)
    {
        ++nInstance;
        
        SPair game(n, m, 0, n-1);
        
        for (int i = 0; i != m; ++i)
        {
            int u, v, cost;
            fscanf(fp, "%d %d %d", &u, &v, &cost);
            
            if (u == v)
                continue;
            else if (u != 0 && u != n-1) // equal neither to the source nor the the sink
                u += n - 1;
            
            game.CreateEdge(u, v, 1, cost);
        }
        
        for (int u = 1; u != n-1; ++u)
            game.CreateEdge(u, u+n-1, 1, 0);
        
        printf("Instance #%d: ", nInstance);
        game.FindPairPath();
    }
    
    return 0;
    
}
