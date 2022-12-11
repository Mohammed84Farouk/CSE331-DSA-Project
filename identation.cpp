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

    while (getline(in,line))
    {
        string openTag;
        string closeTag;
        bool insidetag = false;
        bool atribute = false;
        bool tagPushed = false;
        bool closingtag = false;
        for (int i = 0, j = 0, k = 0 , z = 0; i < line.length(); i++)
        {
            
            if (line[i] == ' ' && openTag.size() == 0){
            }
            else{
                j++;
                if(line[i] == '>'){
                    insidetag = false;
                    atribute = false;
                }
                else if(line[i] == '<' && line[i+1] == '/') closingtag = true;

                if(insidetag && line[i] == ' '){
                    atribute = true;
                }

                if(atribute) continue;
                if(!tagPushed) openTag += line[i];
                if(closingtag) {
                    closeTag += line[i];
                    z++;
                }

            }
            if(i < 3) continue;

            if(openTag[j-1] == '<'){
                k = j-1;
                insidetag = true;
            }

            if (openTag[k] == '<' && openTag[k+1] != '/' && openTag[j-1] == '>' && !tagPushed){
                cout << openTag << endl;
                stack.push(openTag);
                tagPushed = true;
            }
            if (closeTag[0] == '<' && closeTag[1] == '/' && closeTag[z-1] == '>'){
                cout << closeTag << endl;
                if(closeTag != stack.top()) cout << "ERROR "<< openTag<< "Has No Closing Tag " << endl;
                else stack.pop();
            }
        }
    }
}