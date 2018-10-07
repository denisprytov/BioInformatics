#include <iostream>
#include <string>
#include <vector>
#include <algorithm> 
using namespace std;

unsigned kMerSearch(string s1, string s2)
{
    int c = 0;
    for (size_t j = 0; j < s2.size(); ++j)
    {
        for (size_t i = 0; ; ++i)
        {
            if (!s1[i])
            {
                c++;
                break;
            }
            if (s2[i + j] != s1[i])
            {
                break;
            }
        }
    }
    return c;
}

int main()
{
    string s = "", p = "";
    int k = 0, flag = 0;
    unsigned count = 0;
    cin >> s;
    cin >> k;
    vector<string> k_mers;
    vector<string> ans;
    for (auto beg = s.begin(); beg != (s.end() - k + 1); beg++)
    {
        auto cur = beg;
        while(cur != (beg + k))
        {
            p += *cur;
            cur++;
        }
        for(auto km : k_mers)
        {
            if(km == p)
                flag = 1;
        }
        if (!flag)
        {
            k_mers.push_back(p);
        }
        p = "";
        flag = 0;
    }
    for(auto kmer : k_mers)
    {
        int tmp = kMerSearch(kmer, s);
        if(count == tmp)
        {
            ans.push_back(kmer);
            count = tmp;
            continue;
        }
        if(count < tmp)
        {
            ans.clear();
            ans.push_back(kmer);
            count = tmp;
            continue;
        }
    }
    for(size_t i = 0; i < ans.size(); i++)
    {
        cout << ans[i];   
    }
    return 0;
}