#include <vector>
#include <deque>
#include <cstdio>

using namespace std;

struct Edge
{
    int u, v, cap, prev;
};

class Entrapment
{
private:
    int V, s, t, i;
    
    vector<Edge> Residual;
    vector<int> lastEdge;
    
    vector<int> inEdge;
    void BFS(void);
public:
    Entrapment(int N, int M);
    void CreateEdge(int u, int v, int cap);
    void FindPlanet(void);
};


Entrapment::Entrapment(int N, int M) :
V(N), s(0), t(N-1), i(0), Residual(2*M), lastEdge(N, -1), inEdge(N, -1)
{
}

void Entrapment::CreateEdge(int u, int v, int cap)
{
    Residual[i] = {u, v, cap, lastEdge[u]};
    lastEdge[u] = i++;
    
    Residual[i] = {v, u, 0, lastEdge[v]};
    lastEdge[v] = i++;
}

void Entrapment::BFS(void)
{
    deque<int> Q;
    vector<bool> visited(V, false);
    
    visited[s] = true;
    Q.push_back(s);
    
    while (!Q.empty())
    {
        int u = Q.front();
        Q.pop_front();
        
        if (u == t)
            break;
        
        for (int e = lastEdge[u]; e != -1; e = Residual[e].prev)
        {
            int v = Residual[e].v;
            
            if (!visited[v] && Residual[e].cap > 0)
            {
                visited[v] = true;
                inEdge[v] = e;
                Q.push_back(v);
            }
        }
    }
}

void Entrapment::FindPlanet(void)
{
    BFS();
    int planet = -1;
    vector<bool> isInPath(V, false);
    
    for (int v = t; v != s; v = Residual[inEdge[v]].u)
    {
        isInPath[v] = true;
        --Residual[inEdge[v]].cap;
        ++Residual[inEdge[v]^1].cap;
        
        // Dealing with the case where there is only one path from s to t;
        if (s == Residual[inEdge[v]].u)
            planet = v;
    }
    
    // new BFS
    deque<int> Q;
    vector<int> d(V, -1);
    vector<bool> visited(V, false);
    
    visited[s] = true;
    d[s] = 0;
    d[planet] = 1;
    Q.push_back(s);
    
    while (!Q.empty())
    {
        int u = Q.front();
        Q.pop_front();
        
        if (isInPath[u])
            continue;
        
        for (int e = lastEdge[u]; e != -1; e = Residual[e].prev)
        {
            int v = Residual[e].v;
            
            if (!visited[v] && Residual[e].cap > 0)
            {
                visited[v] = true;
                Q.push_back(v);
                d[v] = d[u] + 1;
                
                if (isInPath[v])
                    planet = (d[v] > d[planet]) ? v : planet;
                
                
            }
        }
    }
    
    printf("%d\n", planet+1);
}

int main(void)
{
    FILE *fp = fopen("entrapment.txt", "r");
    
    int nCases, nPlanets, nEdges;
    
    fscanf(fp, "%d", &nCases);
    
    while (nCases--)
    {
        fscanf(fp, "%d %d", &nPlanets, &nEdges);
        ++nPlanets; ++nEdges;
        
        Entrapment game(nPlanets, nEdges);
        
        for (int i = 0; i != nEdges-1; ++i)
        {
            int u, v;
            fscanf(fp, "%d %d", &u, &v);
            --u; --v;
            game.CreateEdge(u, v, 1);
        }
        
        game.CreateEdge(nPlanets-2, nPlanets-1, 1);
        
        game.FindPlanet();
    }
    
    return 0;
}
