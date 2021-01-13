#include <vector>
#include <deque>
#include <algorithm>
#include <cstdio>

using namespace std;

struct Edge
{
    int u, v, cap, cost, prev;
};

const int infty = 100 * 32767;

class AdaCity
{
private:
    
    int V, s, t, i, timeLimit;
    vector<Edge> Residual;
    vector<int> lastEdge;
    vector<vector<int>> edgeId;
    
    bool Levit(vector<int> &inEdge, vector<int> &pi);
public:
    
    AdaCity(int N, int M, int F, int T);
    
    void CreateEdge(int u, int v, int cap, int cost);
    void ConnectInOutVertex(int u, int v, int cap, int cost);
    void MaxCities(void);
};


AdaCity::AdaCity(int N, int M, int F, int T) :
V(2*N + 2), s(0), i(0), t(V-1), Residual(2*(F + N + N + 2*M)), lastEdge(V, -1), edgeId(V, vector<int>(V, -1)), timeLimit(T)
{
}
// F : from source to vertices
// N : from u_out to u_in
// N : from u_int to sink;
// 2*M: the original ones.

void AdaCity::CreateEdge(int u, int v, int cap, int cost)
{
    if (u == s || edgeId[u][v] == -1)
    {
        Residual[i] = {u, v, cap, cost, lastEdge[u]};
        lastEdge[u] = i;
        edgeId[u][v] = i++;
        
        Residual[i] = {v, u, 0, -cost, lastEdge[v]};
        lastEdge[v] = i++;
    }
    else
    {
        int e = edgeId[u][v];
        
        Residual[e].cost = min(Residual[e].cost, cost);
        Residual[e^1].cost = Residual[e].cost;
    }
}

void AdaCity::ConnectInOutVertex(int u, int v, int cap, int cost)
{
    Residual[i] = {u, v, cap, cost, lastEdge[u]};
    lastEdge[u] = i++;
    
    Residual[i] = {v, u, cap, -cost, lastEdge[v]};
    lastEdge[v] = i++;
}

bool AdaCity::Levit(vector<int> &inEdge, vector<int> &pi)
{
    // id: 0 with pi = infty, 1 working on it.
    vector<int> id(V, 0);
    deque<int> Q;
    
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
            
            if (Residual[e].cap > 0 && timeLimit >= pi[u] + Residual[e].cost && pi[v] > pi[u] + Residual[e].cost)
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

void AdaCity::MaxCities(void)
{
    vector<int> inEdge(V, -1);
    vector<int> pi(V, infty);
    int maxFlow = 0;
    
    while (Levit(inEdge, pi))
    {
        ++maxFlow;
        
        for (int v = t; v != s; v = Residual[inEdge[v]].u)
        {
            --Residual[inEdge[v]].cap;
            ++Residual[inEdge[v]^1].cap;
        }
        
        fill(inEdge.begin(), inEdge.end(), -1);
        fill(pi.begin(), pi.end(), infty);
    }
    
    printf("%d\n", maxFlow);
}


int main(void)
{
    FILE *fp = fopen("adacities.txt", "r");
    
    int nCases, N, M, F, T;
    
    fscanf(fp, "%d", &nCases);
    
    while (nCases--)
    {
    
        fscanf(fp, "%d %d %d %d", &N, &M, &F, &T);
        
        AdaCity game(N, M, F, T);
        for (int i = 0; i != F; ++i)
        {
            int u;
            fscanf(fp, "%d", &u);
            game.CreateEdge(0, u, 1, 0);
        }
        
        for (int i = 0; i != M; ++i)
        {
            int u, v, w;
            fscanf(fp, "%d %d %d", &u, &v, &w);
            
            if (u != v)
            {
                game.CreateEdge(u, v+N, infty, w);
                game.CreateEdge(v, u+N, infty, w);
            }
        }
        
        for (int u = 1; u != N+1; ++u)
            game.ConnectInOutVertex(u, u+N, infty, 0);
        
        for (int u = 1; u != N+1; ++u)
            game.CreateEdge(u+N, 2*N+1, 1, 0);
        
        game.MaxCities();
    }
    
    return 0;
    
}


