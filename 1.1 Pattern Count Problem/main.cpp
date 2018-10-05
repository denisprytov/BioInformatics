#include <iostream>
#include <string>
using namespace std;
int main()
{
    string::size_type count = 0;
    size_t flag = 0;
	string s1 = "", s2 = "";
    cin >> s1 >> s2;
    for (size_t j = 0; j < s2.size(); ++j)
    {
        for (size_t i = 0; ; ++i)
        {
            if (!s1[i])
            {
                count++;
                break;
            }
            if (s2[i + j] != s1[i])
            {
                break;
            }
        }
    }
    cout << count;	
	return 0;
}