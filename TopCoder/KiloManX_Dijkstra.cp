#include <vector>
#include <string>
#include <queue>
#include <bitset>
#include <algorithm>
#include <iostream>

using namespace std;

vector<int> visited(32768, false);

struct Node
{
    int weapons;
    int shots;
    
};

bool operator<(const Node &l, const Node &r)
{
    return l.shots > r.shots;
}

class KiloManX
{
private:
public:
    int leastShots(vector<string> damageChart, vector<int> bossHealth);
};


int KiloManX::leastShots(vector<string> damageChart, vector<int> bossHealth)
{
    int nWeapons = int(damageChart.size());
    priority_queue<Node> Q;
    Q.push({0,0});
    
    while (!Q.empty())
    {
        auto node = Q.top();
        Q.pop();
        
        if (visited[node.weapons])
            continue;
        visited[node.weapons] = true;
        
        // check if we have all the weapons. If, so return shots
        if (node.weapons == (1 << nWeapons) - 1)
            return node.shots;
        
        // i: boss we are trying to kill
        for (int i = 0; i != damageChart.size(); ++i)
        {
            // if the boss is already dead then we have his weapon
            if ((node.weapons >> i) & 1)
                continue;
            
            int best = bossHealth[i];
            // j: boss we are trying to use to kil i
            for (int j = 0; j != damageChart.size(); ++j)
            {
                if (i == j)
                    continue;
                
                if (((node.weapons >> j) & 1) && damageChart[j][i] != '0')
                {
                    int shotsNeeded = bossHealth[i] / (damageChart[j][i] - '0');
                    
                    if (bossHealth[i] % (damageChart[j][i] - '0'))
                        ++shotsNeeded;
                    
                    best = min(best, shotsNeeded);
                }
            }
            
            Q.push({node.weapons | 1 << i, node.shots + best});
        }
    }
    
    return 0;
}


int main(void)
{
    KiloManX game;
    
    cout << game.leastShots({"198573618294842",
                            "159819849819205",
                            "698849290010992",
                            "000000000000000",
                            "139581938009384",
                            "158919111891911",
                            "182731827381787",
                            "135788359198718",
                            "187587819218927",
                            "185783759199192",
                            "857819038188122",
                            "897387187472737",
                            "159938981818247",
                            "128974182773177",
                            "135885818282838"}, {157, 1984, 577, 3001, 2003, 2984, 5988, 190003,
        9000, 102930, 5938, 1000000, 1000000, 5892, 38}) << endl;
    
    return 0;
}
