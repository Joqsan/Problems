#include <vector>
#include <string>
#include <deque>
#include <iostream>

using namespace std;

class TeamBuilder
{
private:
    int V;
    vector<int> BFS(int s, vector<string> paths);
public:
    vector<int> specialLocations(vector<string> paths);
};

vector<int> TeamBuilder::BFS(int s, vector<string> paths)
{
    vector<bool> visited(V, false);
    vector<int> predecessor(V, -1);
    deque<int> Q;
    
    visited[s] = true;
    Q.push_back(s);
    
    while (!Q.empty())
    {
        const int &u = Q.front();
        Q.pop_front();
        
        for (int v = 0; v != V; ++v)
            if(v != u && paths[u][v] == '1' && visited[v] == false)
            {
                visited[v] = true;
                predecessor[v] = u;
                Q.push_back(v);
            }
    }
    
    return predecessor;
    
}

vector<int> TeamBuilder::specialLocations(vector<string> paths)
{
    int numLocations = int(paths.size());
    V = numLocations;

    vector<vector<int>> Pi(numLocations);
    
    for (int i = 0; i != numLocations; ++i)
        Pi[i] = BFS(i, paths);
    
    int numAllReachers = 0;
    int numReachableByAll = 0;
    for (int i = 0; i != numLocations; ++i)
    {
        int numNotReachableByI = 0;
        int numNotReachingI = 0;
        for (int j = 0; j != numLocations && (numNotReachableByI < 2 || numNotReachingI < 2); ++j)
        {
            if (Pi[i][j] == -1)
                ++numNotReachableByI;
            
            if (Pi[j][i] == -1)
                ++numNotReachingI;
        }
        
        // if i is reachable by all them a -1 will only in cell Pi[i][i];
        if (numNotReachableByI < 2)
            ++numAllReachers;
        if (numNotReachingI < 2)
            ++numReachableByAll;
            
    }
    
    return {numAllReachers, numReachableByAll};
}

int main(void)
{
    TeamBuilder game;
    
    vector<int> result = game.specialLocations({"0110000","1000100","0000001","0010000","0110000","1000010","0001000"});
    
    for (auto a : result)
        cout << a << " ";
    cout << endl;
    
    return 0;
    
}
