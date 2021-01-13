#include <vector>
#include <set>
#include <deque>
#include <algorithm>
#include <cstdio>

using namespace std;

const int infty = 100 * 32767;

class BankRob
{
private:
    int V, N, s, t; // s_in = s and t_out = t;
    vector<set<int>> Adj;
    vector<vector<int>> cap;
    
    bool BFS(vector<int> &predecessor);
public:
    
    BankRob(int N, int s, int t);
    
    void GetCrossRoads(void);
    void CreateEdge(int u, int v, int c);
    void ConnectInOutVertex(int u, int v, int c);
    
};

BankRob::BankRob(int N, int s, int t) :
V(2*N), N(N), s(s+N), t(t), Adj(V), cap(V, vector<int>(V, 0))
{
}

void BankRob::CreateEdge(int u, int v, int c)
{
    Adj[u].insert(v+N); Adj[v].insert(u+N);
    cap[u][v+N] = cap[v][u+N] = c;
}

void BankRob::ConnectInOutVertex(int u, int v, int c)
{
    Adj[u].insert(v);
    cap[u][v] = c;
}

bool BankRob::BFS(vector<int> &predecessor)
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
            }
    }
    
    return predecessor[t] != -1;
}

void BankRob::GetCrossRoads(void)
{
    vector<int> predecessor(V, -1);
    int nCrossRoads = 0;
    
    // s = s_in = s + N; --> s_out = s - N
    // t = t_out = t; --> t_in = t + N
    cap[s][s-N] = infty;
    cap[t+N][t] = infty;
    
    while (BFS(predecessor))
    {
        ++nCrossRoads;
        for (int v = t; v != s; v = predecessor[v])
        {
            int u = predecessor[v];
            --cap[u][v];
            ++cap[v][u];
        }
        
        fill(predecessor.begin(), predecessor.end(), -1);
    }
    
    printf("%d\n", nCrossRoads);
}

int main(void)
{
    int N, M, s, t;
    
    scanf("%d %d %d %d", &N, &M, &s, &t);
    --s; --t;
    
    BankRob game(N, s, t);
    
    for (int i = 0; i != M; ++i)
    {
        int u, v;
        scanf("%d %d", &u, &v);
        --u; --v;
        
        game.CreateEdge(u, v, 1);
    }
    
    for (int u = 0; u != N; ++u)
        game.ConnectInOutVertex(u+N, u, 1);
    
    game.GetCrossRoads();
    
    return 0;
}
