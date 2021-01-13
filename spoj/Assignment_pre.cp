#include <vector>
#include <algorithm>

using namespace std;

struct Edge
{
    int u, v, cap, cost, prev;
};

const int infty = 100 * 32767;

class Assignment
{
private:
    int V, i;
    vector<Edge> Residual;
    vector<int> lastEdge;
    vector<int> residualCapacityAt;
    bool AugmentPath(vector<int> &fromEdge, vector<int> &pi);
public:
    int s, t;
    Assignment(int m, int n);
    void CreateEdge(int u, int v, int cap, int cost);
    void MinCost(void);
};

Assignment::Assignment(int m, int n) :
V(m + n + 2), i(0), s(0), t(V-1), Residual(2*(m*n + m + n)), lastEdge(V, -1),
residualCapacityAt(V)
{
    residualCapacityAt[s] = infty;
}

void Assignment::CreateEdge(int u, int v, int cap, int cost)
{
    Residual[i] = {u, v, cap, cost, lastEdge[u]};
    lastEdge[u] = i++;
    
    Residual[i] = {v, u, 0, -cost, lastEdge[v]};
    lastEdge[v] = i++;
}

bool Assignment::AugmentPath(vector<int> &fromEdge, vector<int> &pi)
{
    vector<int> Q(V), id(V, 0);
    int qh(0), qt(0);
    
    pi[s] = 0;
    Q[qt++] = s;
    
    while (qh != qt)
    {
        int u = Q[qh++];
        
        if (qh == V)
            qh = 0;
        
        id[u] = 2;
        
        for (int e = lastEdge[u]; e != -1; e = Residual[e].prev)
            if (Residual[e].cap > 0)
            {
                int v = Residual[e].v;
                
                if (pi[v] > pi[u] + Residual[e].cost)
                {
                    pi[v] = pi[u] + Residual[e].cost;
                    fromEdge[v] = e;
                    residualCapacityAt[v] = min(residualCapacityAt[u], Residual[e].cap);
                    
                    if (id[v] == 2)
                    {
                        if (--qh == -1)
                            qh = V-1;
                        
                        Q[qh] = v;
                    }
                    else if (id[v] == 0)
                    {
                        Q[qt++] = v;
                        
                        if (qt == V)
                            qt = 0;
                    }
                    
                    id[v] = 1;
                }
            }
    }
    
    return fromEdge[t] != -1;
}

void Assignment::MinCost(void)
{
    vector<int> fromEdge(V, -1), pi(V, infty);
    int min = 0;
    
    while (AugmentPath(fromEdge, pi))
    {
        int pathMaxFlow = residualCapacityAt[t];
        min += pathMaxFlow * pi[t];
        
        for (int v = t; v != s; v = Residual[fromEdge[v]].u)
        {
            Residual[fromEdge[v]].cap -= pathMaxFlow;
            Residual[fromEdge[v]^1].cap += pathMaxFlow;
        }
        
        fill(fromEdge.begin(), fromEdge.end(), -1);
        fill(pi.begin(), pi.end(), infty);
        
    }
    
    printf("%d\n", min);
}

int main(void)
{
    FILE *fp = fopen("assignment.txt", "r");
    
    int nCases, m, n;
    fscanf(fp, "%d", &nCases);
    
    while (nCases--)
    {
        fscanf(fp, "%d %d", &m, &n);
        Assignment game(m, n);
        
        for (int i = 1; i != m+1; ++i)
        {
            int a;
            fscanf(fp, "%d", &a);
            game.CreateEdge(game.s, i, a, 0);
        }
        
        for (int j = 1; j != n+1; ++j)
        {
            int b;
            fscanf(fp, "%d", &b);
            game.CreateEdge(j+m, game.t, b, 0);
        }
        
        for (int i = 1; i != m+1; ++i)
            for (int j = 1; j != n+1; ++j)
            {
                int w;
                fscanf(fp, "%d", &w);
                game.CreateEdge(i, j+m, infty, w);
            }
        
        game.MinCost();
    }
    
    return 0;
}
