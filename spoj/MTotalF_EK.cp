#include <vector>
#include <set>
#include <deque>
#include <algorithm>
#include <cstdio>
#include <cctype>
#include <string>

using namespace std;

const int infty = 100 * 32767;

int Decode(const char &c)
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
    
    bool BFS(vector<int> &predecessor, int &c_fp);
public:
    
    MTOTALF(int N);
    int V, s, t;
    void CreateEdge(int u, int v, int c);
    void GetMaxFlow(void);
};

MTOTALF::MTOTALF(int N) :
V(52), Adj(52), cap(52, vector<int>(52, 0))
{
}

void MTOTALF::CreateEdge(int u, int v, int c)
{
    Adj[u].insert(v);
    cap[u][v] += c;
}

bool MTOTALF::BFS(vector<int> &predecessor, int &c_fp)
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
        for (auto v : Adj[u])
            if (!visited[v] && cap[u][v] > 0)
            {
                visited[v] = true;
                predecessor[v] = u;
                Q.push_back(v);
                c_fp = min(c_fp, cap[u][v]);
            }
    }
    
    return predecessor[t] != -1;
}

void MTOTALF::GetMaxFlow()
{
    vector<int> predecessor(V, false);
    int max = 0;
    int c_fp = infty;
    
    while (BFS(predecessor, c_fp))
    {
        max += c_fp;
        
        for (int v = t; v != s; v = predecessor[v])
        {
            int u = predecessor[v];
            
            cap[u][v] -= c_fp;
            cap[v][u] += c_fp;
        }
        
        fill(predecessor.begin(), predecessor.end(), -1);
        c_fp = infty;
    }
    
    printf("%d\n", max);
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
                
                if (isspace(c))
                    continue;
                else
                    break;
            }
            
            edge[j] = Decode(c);
        }
        
        scanf("%d\n", &cap);
        
        if (idx[edge[0]] == -1)
            idx[edge[0]] = j++;
        if (idx[edge[1]] == -1)
            idx[edge[1]] = j++;
        
        graph.CreateEdge(idx[edge[0]], idx[edge[1]], cap);
    }
    
    graph.V = j;
    graph.s = idx[0];  // A
    graph.t = idx[25]; // Z
    
    graph.GetMaxFlow();
    
    return 0;
}
