#include <vector>

using namespace std;

struct Edge
{
    int u, v, cap, prev;
};

class AdaCities
{
private:
    int N, s, t, i;
    vector<Edge> Residual;
    vector<int> lastEdge;
    
    
    
public:
    
    void CreateEdge(int u, int v, int cap);
};


void AdaCities::CreateEdge(int u, int v, int cap)
{
    Residual[i] = {u, v+N, cap, lastEdge[u]};
    lastEdge[u] = i++;
    
    Residual[i] = {v+N, u, 0, lastEdge[v+N]};
    lastEdge[v+N] = i++;
    
    
    Residual[i] = {v, u+N, cap, lastEdge[v]};
    lastEdge[v] = i++;
    
    Residual[i] = {u+N, v, 0, lastEdge[u+N]};
    lastEdge[u+N] = i++;
}


