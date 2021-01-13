#include <vector>
#include <queue>
#include <utility>
#include <cstdio>

using namespace std;

int infty = 100 * 32767;

struct Edge
{
    int u, v;
    int cap;
    int cost;
    int prev;
};

class Assignment
{
private:
    
    int V, i;
    
    vector<Edge> Residual;
    vector<int> lastEdge;
    bool AugmentPath(vector<int> &fromEdge, vector<int> &pi);
public:
    
    int s, t;
    Assignment(int m, int n);
    void CreateEdge(int u, int v, int cap, int cost);
    void MinCost(void);
};

Assignment::Assignment(int m, int n) :
V(m + n + 2), Residual(2*(m*n + m + n)), lastEdge(V, -1),
i(0), s(0), t(V-1)
{
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
    queue<int> Q;
    
    pi[s] = 0;
    Q.push(s);
    
    while (!Q.empty())
    {
        int u = Q.front();
        Q.pop();
        
        for (int e = lastEdge[u]; e != -1; e = Residual[e].prev)
            if (Residual[e].cap > 0)
            {
                int v = Residual[e].v;
                
                if (pi[v] > pi[u] + Residual[e].cost)
                {
                    pi[v] = pi[u] + Residual[e].cost;
                    fromEdge[v] = e;
                    Q.push(v);
                }
            }
    }
    
    return fromEdge[t] != -1;
}

void Assignment::MinCost(void)
{
    vector<int> fromEdge(V, -1);
    vector<int> pi(V, infty);
    int min = 0;
    
    while (AugmentPath(fromEdge, pi))
    {
        min += pi[t];
        for (int v = t; v != s; v = Residual[fromEdge[v]].u)
        {
            --Residual[fromEdge[v]].cap;
            ++Residual[fromEdge[v]^1].cap;
        }
        
        fill(fromEdge.begin(), fromEdge.end(), -1);
        fill(pi.begin(), pi.end(), infty);
    }
    
    printf("%d\n", min);
}

int main(void)
{
    int nCases, m, n;
    
    scanf("%d", &nCases);
    
    while (nCases--)
    {
        scanf("%d %d", &m, &n);
        Assignment game(m, n);
        
        for (int i = 1; i != m+1; ++i)
        {
            int u;
            scanf("%d", &u);
            game.CreateEdge(game.s, i, u, 0);
        }
        
        for (int j = 1; j != n+1; ++j)
        {
            int u;
            scanf("%d", &u);
            game.CreateEdge(j+m, game.t, u, 0);
        }
        
        for (int i = 1; i != m+1; ++i)
            for (int j = 1; j != n+1; ++j)
            {
                int u;
                scanf("%d", &u);
                game.CreateEdge(i, j+m, infty, u);
            }
        
        game.MinCost();
    }
    
    return 0;
}
