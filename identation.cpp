#include <iostream>
#include <string>
#include <fstream>
#include <stack>

using namespace std;

int main()
{
    string line;
    ifstream in("test.xml");
    stack<string> stack;

    bool begin_tag = false;
    while (getline(in,line))
    {
        string tmp;
        bool insidetag = false;
        bool atribute = false;
        for (int i = 0, j = 0, k = 0; i < line.length(); i++)
        {
            
            if (line[i] == ' ' && tmp.size() == 0){
            }
            else{
                j++;
                if(line[i] == '>'){
                    insidetag = false;
                    atribute = false;
                }
                if(insidetag && line[i] == ' '){
                    atribute = true;
                }
                if(atribute) continue;
                tmp += line[i];
            }
            if(i < 3) continue;

            if(tmp[j-1] == '<') {
                k = j-1;
                insidetag = true;
            }

            if (tmp[k] == '<' && tmp[k+1] != '/' && tmp[j-1] == '>'){
                cout << tmp << endl;
                stack.push(tmp);
                continue;
            }
        }
    }
}