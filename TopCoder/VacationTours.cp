#include <vector>
#include <string>
#include <cctype>
#include <queue>
#include <limits>
#include <iostream>
#include <algorithm>

using namespace std;

int infty = 100 * numeric_limits<short>::max();

struct Node
{
    int vertex;
    int *distance;
    
    Node(int _v, int *_d)
    {
        vertex = _v;
        distance = _d;
    }
};

bool SortByDistance(const Node &a, const Node &b)
{
    return *a.distance > *b.distance;
}

class VacationsTours
{
private:
    
    int V;
    int s;
    int t;
    
    vector<vector<int>> cost;
    vector<vector<int>> originalCost;
    vector<vector<int>> cap;
    
    int Decode(char c);
    bool Dijkstra(vector<int> &predecessor, vector<int> &pi);
    void ReduceCost(const vector<int> &pi);
    
public:
    int getIncome(vector<string> c, vector<string> d, int fee);
};

int VacationsTours::Decode(char c)
{
    if (isupper(c))
        return c - 'A';
    else if (islower(c))
        return c - 'a' + 26;
    else if (isdigit(c))
        return c - '0' + 52;
    else if (c == '+')
        return 62;
    else
        return 63;
}

bool VacationsTours::Dijkstra(vector<int> &predecessor, vector<int> &pi)
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> Q;
    vector<bool> visited(V, false);
    predecessor[s] = -1;
    pi[s] = 0;
    
    Q.push({pi[s], s});
    while (!Q.empty())
    {
        auto u = Q.top().second;
        Q.pop();
        if (visited[u] == true)
            continue;
        
        visited[u] = true;
        
        if (u == s && cap[u][0] == 0)
            break;
        
        for (int v = 0; v != V; ++v)
            if (u != v && cap[u][v] > 0)
                if (pi[v] > pi[u] + cost[u][v])
                {
                    pi[v] = pi[u] + cost[u][v];
                    predecessor[v] = u;
                    Q.push({pi[v], v});
                }
        
        
    }
    
    return predecessor[t] != -1;
}

void VacationsTours::ReduceCost(const vector<int> &pi)
{
    for (int u = 0; u != V; ++u)
        for (int v = 0; v != V; ++v)
            if (u != v)
                cost[u][v] += pi[u] - pi[v];
}

int VacationsTours::getIncome(vector<string> c, vector<string> d, int fee)
{
    int nSites = int(c.size());
    vector<vector<int>> travelCost(nSites, vector<int>(nSites));
    
    for (int i = 0; i != nSites; ++i)
        for (int j = 0; j != nSites; ++j)
            travelCost[i][j] = Decode(c[i][j]) * 64 + Decode(d[i][j]);
    
    V = 2 * nSites + 1;
    s = V-1;
    t = nSites;
    cap.clear(); cap.resize(V, vector<int>(V, 0));
    cost.clear(); cost.resize(V, vector<int>(V, infty));
    
    // there are V vertices: 0,1,...,nSites-1, nSites,...,V-1
    // 0_out = 0
    // 0_in = nSites;
    // For all v in [1,nSites-1] -> v_in = v and v_out = v+nSites = v_in+nSites
    
    for (int v_in = 1; v_in != nSites; ++v_in)
    {
        // Connecting 0_out with every v_in (cap = 1, cost = travelCost)
        cap[0][v_in] = 1;
        cost[0][v_in] = travelCost[0][v_in];
        cost[v_in][0] = -cost[0][v_in];
        
        // Connecting every v_out = v_in+nSites with 0_in
        int v_out = v_in + nSites;
        cap[v_out][t] = 1;
        cost[v_out][t] = travelCost[v_in][0];
        cost[t][v_out] = -cost[v_out][t];
        
        // Connecting every v_in with its corresponding v_out (cap = 1, cost = 0)
        cap[v_in][v_out] = 1;
        cost[v_in][v_out] = 0;
        cost[v_out][v_in] = -cost[v_in][v_out];
    }
    
    // Connecting every v_out with every v_in
    for (int i = 1; i != nSites; ++i)
        for (int v_in = 1; v_in != nSites; ++v_in)
        {
            int v_out = i + nSites;
            
            if (v_out != v_in + nSites)
            {
                cap[v_out][v_in] = 1;
                cost[v_out][v_in] = travelCost[i][v_in];
                cost[v_in][v_out] = -cost[v_out][v_in];
            }
        }
    
    cost[s][0] = cap[s][0] = 0;
    originalCost = cost;
    
    int income, totalCost, numPaths;
    income = totalCost = numPaths = 0;
    vector<int> predecessor(V, -1);
    vector<int> pi(V, infty);
    vector<vector<int>> flows(V, vector<int>(V, 0));
    
    ++cap[s][0]; ++numPaths;
    while (Dijkstra(predecessor, pi))
    {
        ReduceCost(pi);
        
        int c_fp = infty;
        for (int v = t; v != s; v = predecessor[v])
        {
            c_fp = min(c_fp, cap[predecessor[v]][v]);
            totalCost += originalCost[predecessor[v]][v];
        }
        
        for (int v = t; v != s; v = predecessor[v])
        {
            const int &u = predecessor[v];
            flows[u][v] += c_fp;
            flows[v][u] = -flows[u][v];
            
            cap[u][v] -= c_fp;
            cap[v][u] += c_fp;
        }
        
        if (income >= fee * numPaths - totalCost)
        {
            if (income != 0)
                break;
        }
        else
            income = fee * numPaths - totalCost;
        
        predecessor.clear(); predecessor.resize(V, -1);
        pi.clear(); pi.resize(V, infty);
        ++cap[s][0]; ++numPaths;
    }
    
    return income;
}

int main(void)
{
    VacationsTours game;
    
    cout << game.getIncome({"AAAAA","AAAAA","AAAAA","AAAAA","AAAAA"}, {"AAA//", "/A/BA", "//A/B", "A//A/", "A///A"}, 10) << endl;
    
    return 0;
}

