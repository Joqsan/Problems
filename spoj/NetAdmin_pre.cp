#include <vector>
#include <deque>
#include <algorithm>
#include <cstdio>

using namespace std;

struct Edge
{
    int u, v, cap, prev;
};

class NetAdmin
{
private:
    int V, i, t, k, currentHouse;
    vector<Edge> Residual;
    vector<int> lastEdge;
    
    bool BFS(const int &s, vector<int> &inEdge);
    
public:
    
    void CreateEdge(int u, int v, int cap);
    NetAdmin(int N, int M, int k);
    bool ConnectingHouses(int nColors, bool initCap, const vector<int> houses);
    void FindCableNumber(const vector<int> houses, int lo, int hi);
};

NetAdmin::NetAdmin(int N, int M, int k) :
V(N), i(0), t(0), k(k), Residual(2*M), lastEdge(N, -1), currentHouse(0)
{
}

void NetAdmin::CreateEdge(int u, int v, int cap)
{
    Residual[i] = {u, v, cap, lastEdge[u]};
    lastEdge[u] = i++;
    
    Residual[i] = {v, u, cap, lastEdge[v]};
    lastEdge[v] = i++;
}

bool NetAdmin::BFS(const int &s, vector<int> &inEdge)
{
    deque<int> Q;
    vector<bool> visited(V, false);
    
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
                inEdge[v] = e;
                
                if (v == t)
                    break;
                
                Q.push_back(v);
            }
        }
    }
    
    return visited[t];
}

bool NetAdmin::ConnectingHouses(int nColors, bool initCap, const vector<int> houses)
{
    for (int e = 0; e != i && !initCap; e = 2 + e)
    {
        Residual[e].cap = nColors;
        Residual[e^1].cap = nColors;
    }
    
    vector<int> inEdge(V, -1);
    bool noMoreColors = true;
    
    for (int i = 0; i != k && noMoreColors; ++i)
    {
        if (BFS(houses[i], inEdge))
        {
            for (int v = t; v != houses[i]; v = Residual[inEdge[v]].u)
            {
                --Residual[inEdge[v]].cap;
                ++Residual[inEdge[v]^1].cap;
            }
            
            fill(inEdge.begin(), inEdge.end(), -1);
        }
        else
            noMoreColors = false;
    }
    
    return noMoreColors;
}

void NetAdmin::FindCableNumber(const vector<int> houses, int lo, int hi)
{
    
    while (lo <= hi)
    {
        int currentCap = (hi + lo) / 2;
        
        if (ConnectingHouses(currentCap, false, houses))
            hi = currentCap - 1;
        else
            lo = currentCap + 1;
    }
    
    printf("%d\n", hi + 1);
    
}

int main(void)
{
    FILE *fp = fopen("netadmin.txt", "r");
    int nCases;
    
    fscanf(fp, "%d", &nCases);
    
    while (nCases--)
    {
        int N, M, k;
        
        fscanf(fp, "%d %d %d", &N, &M, &k);
        
        NetAdmin game(N, M, k);
        
        vector<int> houses(k);
        for (int i = 0; i != k; ++i)
        {
            fscanf(fp, "%d", &houses[i]);
            --houses[i];
        }
        
        int initializationCap = (k + 1) / 2;
        for (int i = 0; i != M; ++i)
        {
            int u, v;
            fscanf(fp, "%d %d", &u, &v);
            --u; --v;
            game.CreateEdge(u, v, initializationCap);
        }
        
        if (game.ConnectingHouses(initializationCap, true, houses))
            game.FindCableNumber(houses, 1, initializationCap - 1);
        else
            game.FindCableNumber(houses, initializationCap + 1, k);
    }
    
    return 0;
}
