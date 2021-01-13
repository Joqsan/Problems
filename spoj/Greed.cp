#include <vector>
#include <deque>
#include <cstdio>

using namespace std;

struct Edge
{
    int u, v, cap, flow, cost, prev;
};

const int infty = 100 * 32767;

class Greed
{
private:
  
    int V, i;
    vector<Edge> Residual;
    vector<int> lastEdge;
    
    int AugmentPath(int s, int t, vector<int> &inEdge);
public:
    
    Greed(int N, int M);
    void CreateEdge(int u, int v, int cap, int cost);
    void GetCost(const vector<int> &cards);
    
};

Greed::Greed(int N, int M) :
V(N), i(0), Residual(4*M), lastEdge(N, -1)
{
}

void Greed::CreateEdge(int u, int v, int cap, int cost)
{
    Residual[i] = {u, v, cap, 0, cost, lastEdge[u]};
    lastEdge[u] = i++;
    Residual[i] = {v, u, 0, 0, -cost, lastEdge[v]};
    lastEdge[v] = i++;
    
    Residual[i] = {v, u, cap, 0, cost, lastEdge[v]};
    lastEdge[v] = i++;
    Residual[i] = {u, v, 0, 0, -cost, lastEdge[u]};
    lastEdge[u] = i++;
}

int Greed::AugmentPath(int s, int t, vector<int> &inEdge)
{
    deque<int> Q;
    vector<int> pi(V, infty);
    
    pi[s] = 0;
    Q.push_back(s);
    
    while (!Q.empty())
    {
        int u = Q.front();
        Q.pop_front();
        
        for (int e = lastEdge[u]; e != -1; e = Residual[e].prev)
        {
            int v = Residual[e].v;
            
            if (Residual[e].cap > 0 && pi[v] > pi[u] + Residual[e].cost)
            {
                pi[v] = pi[u] + Residual[e].cost;
                inEdge[v] = e;
                Q.push_back(v);
            }
        }
    }
    
    return pi[t];
}

void Greed::GetCost(const vector<int> &cards)
{
    int cost = 0;
    for (int t = 1; t != V; ++t)
        if (cards[t] != t)
        {
            vector<int> inEdge(V, -1);
            cost += AugmentPath(cards[t], t, inEdge);
            
            for (int v = t; v != cards[t]; v = Residual[inEdge[v]].u)
            {
                --Residual[inEdge[v]].cap;
                ++Residual[inEdge[v]^1].cap;
            }
        }
    
    printf("%d\n", cost);
}

int main(void)
{
    int nCases, nCards, nExchanges;
    
    scanf("%d", &nCases);
    
    while (nCases--)
    {
        scanf("%d", &nCards);
        vector<int> cards(nCards+1);
        
        for (int i = 1; i != nCards+1; ++i)
        {
            int u;
            scanf("%d", &u);
            cards[i] = u;
        }
        
        scanf("%d", &nExchanges);
        
        Greed game(nCards+1, nExchanges);
        
        for (int i = 0; i != nExchanges; ++i)
        {
            int u, v;
            scanf("%d %d", &u, &v);
            game.CreateEdge(u, v, infty, 1);
        }
        
        game.GetCost(cards);
    }
    
    return 0;
}
