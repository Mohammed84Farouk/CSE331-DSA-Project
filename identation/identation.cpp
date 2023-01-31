#include <iostream>
#include <string>
#include <fstream>
#include <stack>

using namespace std;
void pretify(string input, string output);
void consistent(string input, string output){
    stack<string> stack;
    bool stackempty = true;
    ifstream in(input);
    ofstream out(output);
    string line;
    while (getline(in,line))
    {
        string openTag;
        string closeTag;
        string text;
        bool tagPushed = true;
        bool closingtag = false;
        for (int i = 0, j = 0, z = 0; i < line.length(); i++)
        {
            if(line[i] == '<' && line[i+1] == '/') closingtag = true;
            else if(line[i] == '<' && line[i+1] != '/' && line[i+1] != '?' ) {
                tagPushed = false;
            }
            if(closingtag) {
                closeTag += line[i];
                z++;
            }
            else if(!tagPushed){
                openTag += line[i];
                j++;
            }
            else if(!closingtag) text += line[i];
            if(i < 3) continue;


            if (openTag[0] == '<' && openTag[1] != '/' && openTag[j-1] == '>' && !tagPushed){
                if(openTag[1] != '?') stack.push(openTag.substr(1,j-2));
                stackempty = false;
                tagPushed = true;

                out<<endl;
                out << openTag.substr(0,j-1) << '>';
            }
            if (closeTag[0] == '<' && closeTag[1] == '/' && closeTag[z-1] == '>'){
                out << text ;
                if(stackempty){
                    out<<endl;
                    out << '<' << closeTag.substr(2,z-3) << '>';
                    out << '<' << '/' << closeTag.substr(2,z-3) << '>';
                }
                else if(closeTag.substr(2, z-3) != stack.top()){
                    out << '<' << '/' << stack.top() << '>';
                    out <<endl;
                    stack.pop();
                    stackempty = stack.empty();
                    if(!stackempty && closeTag.substr(2, z-3) == stack.top()){
                        out << closeTag;
                        stack.pop();
                        stackempty = stack.empty();
                    }
                    else {
                        out << '<' << closeTag.substr(2,z-3) << '>';
                        out << '<' << '/' << closeTag.substr(2,z-3) << '>';
                    }
                    continue;
                } 
                else{
                    if(text.size() < 1){
                        out << endl;
                    } 
                    out << closeTag;
                    stack.pop();
                    stackempty = stack.empty();
                }
            }else if (i == line.length() -1) out << text;
        }
    }
}
void pretify(string input, string output){
    int depth = 0;
    int depthfornextline = -1;
    stack<string> stack;
    ifstream in(input);
    ofstream out(output);
    string line;
    while (getline(in,line))
    {
        string openTag;
        string closeTag;
        string text;
        bool tagPushed = true;
        bool closingtag = false;
        for (int i = 0, j = 0, z = 0; i < line.length(); i++)
        {
            if (line[i] == ' ' && (openTag.size() == 0 || text.size() == 0)){
                continue;
            }
            if(line[i] == '<' && line[i+1] == '/') {
                closingtag = true;
                z = 0;
                closeTag = "";
            }
            else if(line[i] == '<' && line[i+1] != '/' && line[i+1] != '?' ) {
                tagPushed = false;
                j = 0;
                openTag = "";
            }
            if(closingtag) {
                closeTag += line[i];
                z++;
            }
            else if(!tagPushed){
                openTag += line[i];
                j++;
            }
            else if(!closingtag) text += line[i];
            if(i < 3) continue;

            if (openTag[0] == '<' && openTag[1] != '/' && openTag[j-1] == '>' && !tagPushed){
                depth++;
                if(depth == depthfornextline) out<<endl;
                if(openTag[1] != '?') stack.push(openTag.substr(1,j-2));
                depthfornextline++;
                tagPushed = true;
                depth = depthfornextline-1;
                for (int w = 0; w < depthfornextline; w++)
                {
                    out<<'\t';
                }
                out << openTag.substr(0,j-1) << '>';
            }
            if (closeTag[0] == '<' && closeTag[1] == '/' && closeTag[z-1] == '>'){
                out<<endl;
                for (int w = 0; w < depthfornextline+1; w++)
                {
                    out<<'\t';
                }
                out<< text <<endl;
                text = "";
                depthfornextline--;
                for (int w = 0; w < depthfornextline+1; w++)
                {
                    out<<'\t';
                }
                out << closeTag << endl;

            }else if (i == line.length() -1){
                for (int w = 0; w < depthfornextline; w++)
                {
                    out<<'\t';
                }
                out << text <<endl;
                text = "";
            }
        } 
    }
}