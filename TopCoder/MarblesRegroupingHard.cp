#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <queue>
#include <limits>
#include <iostream>

using namespace std;

int infty = 10 * numeric_limits<short>::max();

struct Node
{
    int configColors;
    int configBoxes;
    int numMoves;
};

bool operator<(const Node &a, const Node &b)
{
    return a.numMoves > b.numMoves;
}

class MarblesRegroupingHard
{
private:
public:
    int minMoves(vector<string> boxes);
};

int MarblesRegroupingHard::minMoves(vector<string> boxes)
{
    int numBoxes = int(boxes.size());
    int numColors = int(count(boxes[0].begin(), boxes[0].end(), ' ')) + 1;
    
    vector<vector<int>> Boxes(numBoxes, vector<int>(numColors, 0));
    vector<int> marblesPerColor(numColors, 0);
    for (int i = 0; i != numBoxes; ++i)
    {
        stringstream stream(boxes[i]);
        int j = 0;
        int nMarbles;
        while (stream >> nMarbles)
        {
            Boxes[i][j] = nMarbles;
            marblesPerColor[j] += nMarbles;
            ++j;
        }
    }
    
    priority_queue<Node> Q;
    vector<bool> visited((1 << numColors), false);
    Q.push({0, 0, 0});
    
    while (!Q.empty())
    {
        auto node = Q.top();
        Q.pop();
        
        if (visited[node.configColors] == true)
            continue;
        
        visited[node.configColors] = true;
        
        // check if all marbles are in their respective boxes
        if (node.configColors == (1 << numColors) - 1)
            return node.numMoves;
        
        for (int i = 0; i != numColors; ++i)
        {
            if ((node.configColors >> i) & 1)
                continue;
            
            int selectedBox = 0;
            int best = infty;
            for (int j = 0; j != numBoxes; ++j)
            {
                if ((node.configBoxes >> j) & 1)
                    continue;
                
                if (marblesPerColor[i] - Boxes[j][i] < best)
                {
                    best = marblesPerColor[i] - Boxes[j][i];
                    selectedBox = j;
                }
            }
            
            Q.push({node.configColors | 1 << i, node.configBoxes | 1 << selectedBox, node.numMoves + best});
                    
        }
        
    }
    
    return 0;
}

int main(void)
{
    MarblesRegroupingHard game;
    
    cout << game.minMoves({"97 94 0 99","1 72 46 45","0 10 47 75","0 92 76 20","2 25 98 22"}) << endl;
    
    return 0;
}
