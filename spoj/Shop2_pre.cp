#include <vector>
#include <deque>
#include <algorithm>
#include <cstdio>
#include <ctime>

using namespace std;

struct Edge
{
    int u, v, cap, prev;
};

const int infty = int(1e9);

class Graph
{
private:
    int V, s, t;
    vector<Edge> Residual;
    vector<int> lastEdge;
    
    
public:
    int i, coveredByVouchers;
    Graph(int N, int M);
    void CreateEdge(int u, int v, int cap);
    
    bool BFS(void);
    void Reflow(const int &flow, const vector<int> &inEdge);
};

Graph::Graph(int N, int M) :
V(N + M + 2), Residual(2*(N*M + N + M)), lastEdge(V, -1), s(0), t(V-1), coveredByVouchers(0), i(0)
{
}

void Graph::CreateEdge(int u, int v, int cap)
{
    Residual[i] = {u, v, cap, lastEdge[u]};
    lastEdge[u] = i++;
    
    Residual[i] = {v, u, 0, lastEdge[v]};
    lastEdge[v] = i++;
}

bool Graph::BFS(void)
{
    deque<int> Q;
    vector<int> flow(V, 0);
    vector<int> inEdge(V, -1);
    
    flow[s] = infty;
    Q.push_back(s);
    
    while (!Q.empty())
    {
        int u = Q.front();
        Q.pop_front();
        
        for (int e = lastEdge[u]; e != -1; e = Residual[e].prev)
        {
            int v = Residual[e].v;
            
            if (Residual[e].cap > 0 && flow[v] == 0)
            {
                flow[v] = min(flow[u], Residual[e].cap);
                inEdge[v] = e;
                Q.push_back(v);
            }
        }
    }
    
    if (flow[t] > 0)
    {
        Reflow(flow[t], inEdge);
        return true;
    }
    else
        return false;
}

void Graph::Reflow(const int &flow, const vector<int> &inEdge)
{
    coveredByVouchers += flow;
    
    for (int v = t; v != s; v = Residual[inEdge[v]].u)
    {
        Residual[inEdge[v]].cap -= flow;
        Residual[inEdge[v]^1].cap += flow;
    }
}

int main(void)
{
    FILE *fp = fopen("shop2.txt", "r");
    int nCases;
    
    fscanf(fp, "%d", &nCases);
    
    while (nCases--)
    {
        int N, M, flow;
        fscanf(fp, "%d %d", &N, &M);
        Graph game(N, M);
        
        int t = N + M + 1;
        
        vector<int> itemPrice(N), vouchers(M), inEdge(N+M+2, -1);
        int totalPrice = 0;
        
        for (int u = 1; u != N+1; ++u)
        {
            fscanf(fp, "%d", &itemPrice[u-1]);
            game.CreateEdge(0, u, itemPrice[u-1]);
            totalPrice += itemPrice[u-1];
        }
        
        for (int v = 1; v != M+1; ++v)
        {
            fscanf(fp, "%d", &vouchers[v-1]);
            game.CreateEdge(v+N, t, vouchers[v-1]);
        }
        
        for (int v = 1; v != M+1; ++v)
        {
            int m;
            fscanf(fp, "%d", &m);
            
            while (m--)
            {
                int u;
                fscanf(fp, "%d", &u);
                game.CreateEdge(u, v+N, infty);
                
                flow = min(itemPrice[u-1], vouchers[v-1]);
                
                if (flow > 0)
                {
                    itemPrice[u-1] -= flow;
                    vouchers[v-1] -= flow;
                    
                    inEdge[t] = 2 * N + 2 * (v-1);
                    inEdge[v+N] = game.i - 2;
                    inEdge[u] = 2 * (u-1);
                    inEdge[0] = -1;
                    
                    game.Reflow(flow, inEdge);
                }
                
            }
        }
        
        while (game.BFS());
        
        printf("%d\n", totalPrice - game.coveredByVouchers);
        
    }
    
    return 0;
    
    
}

