#include <vector>
#include <deque>
#include <algorithm>
#include <set>
#include <cctype>
#include <cstdio>

using namespace std;

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
    vector<set<int>> Adj;
    vector<vector<int>> cap;
    
    // Push
    vector<int> nVerticesLayer, excess, h;
    int b;
    vector<deque<int>> Layers;
    vector<bool> active;
    
    void Discharge(const int &u);
    void Relabel(const int &u);
    void Push(const int &u, const int &v);
    
    void CheckActivity(const int &u);
    bool CheckGap(const int &u, int heightChecked);
public:
    int V, s, t;
    MTOTALF(int N);
    
    void CreateEdge(int u, int v, int cap);
    void GetFlow(void);
};

MTOTALF::MTOTALF(int N) :
Adj(52), cap(52, vector<int>(52, 0)),
nVerticesLayer(52, 0), excess(52, 0), h(52, 0), Layers(52), active(52, false)
{
}

void MTOTALF::CreateEdge(int u, int v, int c)
{
    Adj[u].insert(v); Adj[v].insert(u);
    cap[u][v] += c;
    cap[v][u] += 0;
}

void MTOTALF::Discharge(const int &u)
{
    for (auto v : Adj[u])
    {
        if (excess[u] > 0)
        {
            if (cap[u][v] > 0 && h[u] == h[v] + 1)
                Push(u, v);
        }
        else
            break;
    }
    
    
    if (excess[u] > 0)
        Relabel(u);
}

void MTOTALF::Push(const int &u, const int &v)
{
    int delta = min(excess[u], cap[u][v]);
    
    cap[u][v] -= delta;
    cap[v][u] += delta;
    
    excess[u] -= delta;
    excess[v] += delta;
    
    CheckActivity(u);
    CheckActivity(v);
}

void MTOTALF::CheckActivity(const int &u)
{
    if (excess[u] != 0 && !active[u])
    {
        active[u] = true;
        Layers[h[u]].push_back(u);
    }
    else if (excess[u] == 0 && active[u])
    {
        active[u] = false;
        Layers[h[u]].pop_front();
    }
}

void MTOTALF::Relabel(const int &u)
{
    int oldHeight = h[u];
    h[u] = V;
    
    for (auto v : Adj[u])
        if (cap[u][v] > 0)
            h[u] = min(h[u], h[v] + 1);
    
    
    Layers[oldHeight].pop_front();
    --nVerticesLayer[oldHeight];
    
    if (CheckGap(u, oldHeight))
        b = oldHeight - 1;
    else
    {
        if (h[u] != V)
        {
            b = h[u];
            Layers[h[u]].push_back(u);
        }
        else
            active[u] = false;
        
        ++nVerticesLayer[h[u]];
    }
}

bool MTOTALF::CheckGap(const int &u, int heightChecked)
{
    if (heightChecked != 0 && nVerticesLayer[heightChecked] == 0)
    {
        for (int u = 0; u != V; ++u)
            if (h[u] > heightChecked && h[u] < V)
            {
                active[u] = false;
                --nVerticesLayer[h[u]];
                
                h[u] = V;
                ++nVerticesLayer[V];
            }
        
        return true;
    }
    else
        return false;
}

void MTOTALF::GetFlow(void)
{
    b = 0;
    active[s] = true;
    for (auto v : Adj[s])
        excess[s] += cap[s][v];
    
    Layers[b].push_back(s);
    while (b != -1)
    {
        if (!Layers[b].empty())
        {
            int u = Layers[b].front();
            
            if (u != t)
                Discharge(u);
            else
                Layers[b].pop_front();
        }
        else
            --b;
    }
    
    printf("%d\n", excess[t]);
}

int main(void)
{
    int N;

    scanf("%d", &N);
    
    MTOTALF graph(N);
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
                c = getchar();
                
                if (!isspace(c))
                    break;
            }
            
            edge[j] = Decode(c);
        }
        
        scanf("%d", &cap);
        
        if (idx[edge[0]] == -1)
            idx[edge[0]] = j++;
        if (idx[edge[1]] == -1)
            idx[edge[1]] = j++;
        
        graph.CreateEdge(idx[edge[0]], idx[edge[1]], cap);
    }
    
    graph.V = j;
    graph.s = idx[0];  // A
    graph.t = idx[25]; // Z
    
    graph.GetFlow();
    
    return 0;
}
