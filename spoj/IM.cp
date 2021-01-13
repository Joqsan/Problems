#include <vector>
#include <deque>
#include <algorithm>
#include <cstdio>

using namespace std;

struct Edge
{
    int u, v, cap, prev;
};

bool AreInRange(const int &N, const int &u, const int &v)
{
    return u >= 0 && u < N && v >= 0 && v < N;
}

class IGMap
{
private:
    int V, i;
    vector<Edge> Residual;
    vector<int> lastEdge;
    
    bool AugmentPath(vector<int> &inEdge);
public:
    
    int s, t;
    IGMap(int n, int m);
    void FindValidPath(void);
    void CreateEdge(int u, int v, int cap);
    void ConnectToSink(int u, int cap);
    void ConnectInOutVertex(int u, int v, int cap);
};

IGMap::IGMap(int n, int m) :
V(2*(n-1) + 2), Residual(2*(2*m + (n-1) + 2)), lastEdge(V+1, -1),
s(1), t(V), i(0)
{
}

void IGMap::ConnectInOutVertex(int u, int v, int cap)
{
    Residual[i] = {u, v, cap, lastEdge[u]};
    lastEdge[u] = i++;
    
    Residual[i] = {v, u, 0, lastEdge[v]};
    lastEdge[v] = i++;
}

void IGMap::ConnectToSink(int u, int cap)
{
    Residual[i] = {u, t, cap, lastEdge[u]};
    lastEdge[u] = i++;
    
    Residual[i] = {t, u, 0, lastEdge[t]};
    lastEdge[t] = i++;
}

void IGMap::CreateEdge(int u, int v, int cap)
{
    Residual[i] = {u, v, cap, lastEdge[u]};
    lastEdge[u] = i++;
    
    Residual[i] = {v, u, 0, lastEdge[v]};
    lastEdge[v] = i++;
    
    Residual[i] = {v, u, cap, lastEdge[v]};
    lastEdge[v] = i++;
    
    Residual[i] = {u, v, 0, lastEdge[u]};
    lastEdge[u] = i++;
}

bool IGMap::AugmentPath(vector<int> &inEdge)
{
    deque<int> Q;
    vector<bool> visited(V+1, false);
    
    visited[s] = true;
    Q.push_back(s);
    
    while (!Q.empty())
    {
        int u = Q.front();
        Q.pop_front();
        
        for (int e = lastEdge[u]; e != -1; e = Residual[e].prev)
        {
            int v = Residual[e].v;
            
            if (!visited[v] && Residual[e].cap)
            {
                visited[v] = true;
                inEdge[v] = e;
                Q.push_back(v);
            }
        }
    }
    
    return visited[t];
}

void IGMap::FindValidPath(void)
{
    vector<int> inEdge(V+1, -1);
    
    ConnectToSink(0, 1);
    ConnectToSink(2, 1);
    
    int nPaths = 0;
    for (int i = 0; i != 2; ++i)
    {
        if (AugmentPath(inEdge))
        {
            ++nPaths;
            
            if (nPaths != 2)
            {
                for (int v = t; v != s; v = Residual[inEdge[v]].u)
                {
                    --Residual[inEdge[v]].cap;
                    ++Residual[inEdge[v]^1].cap;
                }
            
                fill(inEdge.begin(), inEdge.end(), -1);
            }
        }
    }
    
    if (nPaths == 2)
        printf("YES\n");
    else
        printf("NO\n");
}

int main(void)
{
    int nCases, n, m;
    
    scanf("%d", &nCases);
    
    while (nCases--)
    {
        scanf("%d %d", &n, &m);
        
        IGMap game(n, m);
        
        for (int i = 0; i != m; ++i)
        {
            int u, v;
            scanf("%d %d", &u, &v);
            --u; --v;
            
            if (AreInRange(n, u, v))
            {
                if (v != game.s)
                    v += n;
                
                game.CreateEdge(u, v, 1);
            }
        }
        
        for (int u = 0; u != n; ++u)
            if (u != game.s)
                game.ConnectInOutVertex(u+n, u, 1);
        
        game.FindValidPath();
    }
    
    return 0;
    
}
