#include <vector>
#include <queue>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <cctype>


using namespace std;

const int infty = 100 * 32767;

struct Edge
{
    int u, v, cap, flow, prev;
};

int Decode(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c - 'A';
    else
        return c - 'a' + 26;
}

class MTOTALF
{
private:
    int i;
    vector<Edge> Residual;
    vector<int> lastEdge, currentArc, excess, h;
    queue<int> Q;
    
    void Discharge(const int &u);
    void Push(const int &e);
    void Relabel(const int &u);
public:
    int V, s, t;
    MTOTALF(int m);
    void CreateEdge(int u, int v, int cap);
    void RelabelToFront(void);
};

MTOTALF::MTOTALF(int m) :
i(0), Residual(2*m), lastEdge(52, -1), excess(52, 0), h(52, 0)
{
}

void MTOTALF::CreateEdge(int u, int v, int cap)
{
    Residual[i] = {u, v, cap, 0, lastEdge[u]};
    lastEdge[u] = i++;
    
    Residual[i] = {v, u, 0, 0, lastEdge[v]};
    lastEdge[v] = i++;
}

void MTOTALF::Push(const int &e)
{
    int delta = min(excess[Residual[e].u], Residual[e].cap);
    
    Residual[e].flow += delta;
    Residual[e^1].flow = -Residual[e].flow;
    
    Residual[e].cap -= delta;
    Residual[e^1].cap += delta;
    
    excess[Residual[e].u] -= delta;
    excess[Residual[e].v] += delta;
    
    if (delta > 0 && excess[Residual[e].v] == delta)
        Q.push(Residual[e].v);
}

void MTOTALF::Relabel(const int &u)
{
    int minHeight = infty;
    
    for (int e = lastEdge[u]; e != -1; e = Residual[e].prev)
        if (Residual[e].cap > 0 && h[u] <= h[Residual[e].v])
            minHeight = min(minHeight, h[Residual[e].v]);
    
    h[u] = 1 + minHeight;
}

void MTOTALF::Discharge(const int &u)
{
    while (excess[u] > 0)
    {
        int e = currentArc[u];
        
        if (e == -1)
        {
            Relabel(u);
            currentArc[u] = lastEdge[u];
        }
        else if (Residual[e].cap > 0 && h[u] == h[Residual[e].v] + 1)
            Push(e);
        else
            currentArc[u] = Residual[e].prev;
    }
}

void MTOTALF::RelabelToFront(void)
{
    h[s] = V;
    excess[s] = infty;
    for (int e = lastEdge[s]; e != -1; e = Residual[e].prev)
        Push(e);
    
    currentArc = lastEdge;
    
    while (!Q.empty())
    {
        int u = Q.front(); Q.pop();
        
        if (u != s && u != t)
            Discharge(u);
    }
    
    int maxFlow = 0;
    for (int e = lastEdge[s]; e != -1; e = Residual[e].prev)
        maxFlow += Residual[e].flow;
    
    printf("%d\n", maxFlow);
}

int main(void)
{
    ifstream myfile("mtotalf.txt");
    
    int N;
    myfile >> N;
    
    MTOTALF MTOTALF(N);
    vector<int> idx(52, -1);
    int j = 0;
    for (int i = 0; i != N; ++i)
    {
        vector<int> edge(2);
        int cap;
        for (int j = 0; j != 2; ++j)
        {
            char c;
            while (true)
            {
                myfile.get(c);
                
                if (isspace(c))
                    continue;
                else
                    break;
            }
            
            edge[j] = Decode(c);
        }
        
        myfile >> cap;
        
        if (idx[edge[0]] == -1)
            idx[edge[0]] = j++;
        if (idx[edge[1]] == -1)
            idx[edge[1]] = j++;
        
        MTOTALF.CreateEdge(idx[edge[0]], idx[edge[1]], cap);
    }
    
    MTOTALF.V = j;
    MTOTALF.s = idx[0];  // A
    MTOTALF.t = idx[25]; // Z
    
    MTOTALF.RelabelToFront();
    
    return 0;
}

