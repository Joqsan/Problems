#include <vector>
#include <deque>
#include <cstdio>

using namespace std;

class Bug
{
private:
    int V;
    vector<vector<int>> Adj;
    vector<int> color;
    bool BFS(int source);
public:
    
    Bug(int N);
    void CreateEdge(int u, int v);
    void FindSuspicious(void);
};

void Bug::CreateEdge(int u, int v)
{
    Adj[u].push_back(v);
    Adj[v].push_back(u);
}

Bug::Bug(int N) :
V(N), Adj(N), color(N, 0)
{
}

bool Bug::BFS(int source)
{
    deque<int> Q;
    bool gayFound = false;
    
    color[source] = 1;
    Q.push_back(source);
    
    int i = 0;
    while (!gayFound && !Q.empty())
    {
        int u = Q.front();
        Q.pop_front();
        i++;
        for (auto v : Adj[u])
        {
            if (color[v] == 0)
            {
                color[v] = (color[u] == 1) ? 2 : 1;
                Q.push_back(v);
            }
            else if (color[v] == color[u])
            {
                gayFound = true;
                break;
            }
        }
    }
    
    return gayFound;
}

void Bug::FindSuspicious(void)
{
    bool gayFound = false;
    for (int u = 0; u != V && !gayFound; ++u)
        if (color[u] == 0)
            gayFound = BFS(u);
    
    if (gayFound)
        printf("Suspicious bugs found!\n");
    else
        printf("No suspicious bugs found!\n");
}

int main(void)
{
    int nCases, nBugs, nInteractions;
    
    scanf("%d", &nCases);
    
    int i = 0;
    while (i != nCases)
    {
        scanf("%d %d", &nBugs, &nInteractions);
        
        Bug game(nBugs);
        
        for (int i = 0; i != nInteractions; ++i)
        {
            int u, v;
            scanf("%d %d", &u, &v);
            --u; --v;
            game.CreateEdge(u, v);
        }
        
        printf("Scenario #%d:\n", ++i);
        game.FindSuspicious();
    }
    
    return 0;
}

