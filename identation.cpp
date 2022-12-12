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
    ofstream out("output.xml");
    int depth = 0;
    while (getline(in,line))
    {
        string openTag;
        string closeTag;
        string text;
        string attribute;
        bool insidetag = false;
        bool atribute = false;
        bool tagPushed = true;
        bool closingtag = false;
        for (int i = 0, j = 0, z = 0; i < line.length(); i++)
        {
            if (line[i] == ' ' && openTag.size() == 0){
            }
            else{
                if(line[i] == '>'){
                    insidetag = false;
                    atribute = false;
                }
                else if(line[i] == '<' && line[i+1] == '/') closingtag = true;
                else if(line[i] == '<' && line[i+1] != '/' && line[i+1] != '?' ) {
                    insidetag = true;
                    tagPushed = false;
                }

                if(insidetag && line[i] == ' '){
                    atribute = true;
                }

                if(atribute) attribute+= line[i];
                
                else if(closingtag) {
                    closeTag += line[i];
                    z++;
                }
                else if(!tagPushed){
                    openTag += line[i];
                    j++;
                }
                else if(!insidetag && !closingtag) text += line[i];
            }
            if(i < 3) continue;


            if (openTag[0] == '<' && openTag[1] != '/' && openTag[j-1] == '>' && !tagPushed){
                if(openTag[1] != '?') stack.push(openTag.substr(1,j-2));
                tagPushed = true;
                if(depth < 3) out<<endl;
                for (int x = 0; x < depth * 4; x++)
                {
                    out<< ' ';
                }
                out << openTag.substr(0,j-1) << attribute << '>';
                depth++;
            }
            if (closeTag[0] == '<' && closeTag[1] == '/' && closeTag[z-1] == '>'){
                out << text ;
                if(closeTag.substr(2, z-3) != stack.top()){
                    out << '<' << '/' << stack.top() << '>' << endl;
                    depth -= 2;
                    for (int x = 0; x < depth * 4; x++)
                    {
                        out<< ' ';
                    }
                    out << closeTag;
                    continue;
                } 
                else{
                    out << closeTag <<endl;
                    stack.pop();
                }
                depth--;
            }else if (i == line.length() -1) out << text;
        }
    }
}