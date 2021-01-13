#include <vector>
#include <deque>
#include <utility>
#include <cstdio>
#include <cctype>

using namespace std;

typedef pair<int, int> cell;

vector<int> dc = {1, 1, 0, -1, -1, -1, 0, 1};
vector<int> dr = {0, -1, -1, -1, 0, 1, 1, 1};

bool IsInside(int R, int r, int C, int c)
{
    return r >= 0 && r < R && c >= 0 && c < C;
}

class ABCPath
{
private:
    int R, C;
    int max;
    
public:
    
    ABCPath(int R, int C);
    void FindLongestPath(deque<cell> &Apos, const vector<vector<char>> &Grid);
};

ABCPath::ABCPath(int R, int C) :
R(R), C(C)
{
}

void ABCPath::FindLongestPath(deque<cell> &Apos, const vector<vector<char>> &Grid)
{
    
    if (!Apos.empty())
    {
        deque<cell> Q;
        vector<vector<int>> time(R, vector<int>(C, -1));
        int maxTime = 1;
        while (!Apos.empty())
        {
            vector<vector<bool>> allChildrenVisited(R, vector<bool>(C, false));
            int a1 = Apos.front().first;
            int a2 = Apos.front().second;
            
            Q.push_back({a1, a2});
            Apos.pop_front();
            
            time[a1][a2] = 1;
            while (!Q.empty())
            {
                int u1 = Q.front().first;
                int u2 = Q.front().second;
                
                if (allChildrenVisited[u1][u2])
                {
                    if (maxTime < time[u1][u2])
                        maxTime = time[u1][u2];
                    
                    Q.pop_front();
                }
                else
                {
                    for (int i = 0; i != 8; ++i)
                    {
                        int v1 = u1 + dr[i];
                        int v2 = u2 + dc[i];
                        
                        if (IsInside(R, v1, C, v2) && Grid[v1][v2] == Grid[u1][u2]+1 && time[v1][v2] == -1)
                        {
                            time[v1][v2] = time[u1][u2] + 1;
                            Q.push_front({v1, v2});
                        }
                        
                    }
                    
                    allChildrenVisited[u1][u2] = true;
                }
            }
        }
        
        printf("%d\n", maxTime);
    }
    else
        printf("%d\n", 0);
}

int main(void)
{
    int R, C, i;
    i = 0;
    while (scanf("%d %d", &R, &C) == 2 && R != 0)
    {
        vector<vector<char>> Grid(R, vector<char>(C));
        deque<cell> Apos;
        ABCPath game(R, C);
        
        getchar();
        for (int i = 0; i != R; ++i)
        {
            char c; int j = 0;
            while (!isspace(c = getchar()))
            {
                if (c == 'A')
                    Apos.push_back({i, j});
                
                Grid[i][j++] = c;
            }
        }
        
        printf("Case %d: ", ++i);
        game.FindLongestPath(Apos, Grid);
    }
    
    return 0;
}
