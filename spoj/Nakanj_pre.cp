#include <vector>
#include <deque>
#include <utility>
#include <cstdio>
#include <cctype>

using namespace std;

typedef pair<int, int> coord;

vector<int> dc = {2, 2, 1, -1, -2, -2, 1, -1};
vector<int> dr = {-1, 1, 2, 2, -1, 1, -2, -2};

bool IsInside(int N, int r, int c)
{
    return r >= 0 && r < N && c >= 0 && c < N;
}

void BFS(coord s, coord t)
{
    deque<coord> Q;
    vector<vector<int>> distance(8, vector<int>(8, -1));
    
    distance[s.first][s.second] = 0;
    Q.push_back(s);
    
    int t1 = t.first; int t2 = t.second;
    while (distance[t1][t2] == -1 && !Q.empty())
    {
        int ur = Q.front().first;
        int uc = Q.front().second;
        Q.pop_front();
        
        for (int i = 0; i != 8 && distance[t1][t2] == -1; ++i)
        {
            int vr = ur + dr[i];
            int vc = uc + dc[i];
            
            if (IsInside(8, vr, vc) && distance[vr][vc] == -1)
            {
                distance[vr][vc] = distance[ur][uc] + 1;
                Q.push_back({vr, vc});
            }
        }
        
    }
    
    printf("%d\n", distance[t1][t2]);
}

int main(void)
{
    FILE *fp = fopen("nakanj.txt", "r");
    
    int nCases;
    
    fscanf(fp, "%d", &nCases);
    
    while (nCases--)
    {
        char s1, s2, t1, t2;
        
        while (isspace(s1 = getc(fp)));
        
        s2 = getc(fp);
        getc(fp);
        t1 = getc(fp); t2 = getc(fp);
        
        BFS({8 - (s2 - '0'), s1 - 'a'}, {8 - (t2 - '0'), t1 - 'a'});
        
    }
    
    return 0;
}
