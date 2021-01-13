#include <vector>
#include <limits>
#include <iostream>

using namespace std;

void PrintCoinsUsed(int current, const vector<int> &coin_added)
{
    if (current == 0)
        ;
    else
    {
        cout << coin_added[current] << " ";
        PrintCoinsUsed(current - coin_added[current], coin_added);
    }
}

void FindMinCoins(int s, vector<int> coin_values)
{
    vector<int> min(s+1, numeric_limits<short>::max());
    vector<int> coin_added(s+1, 0);
    min[0] = 0;
    
    for (int i = 0; i != s+1; ++i)
        for (int j = 0; j != coin_values.size(); ++j)
            if (coin_values[j] <= i && min[i - coin_values[j]] + 1 < min[i])
            {
                min[i] = min[i - coin_values[j]] + 1;
                coin_added[i] = coin_values[j];
            }
    
    cout << min[s] << endl;
    PrintCoinsUsed(s, coin_added);
    cout << endl;
}

int main(void)
{
    FindMinCoins(11, {1, 3, 5});
    
    return 0;
}
