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
            if(line[i] == '<' && line[i+1] == '/') {
                closingtag = true;
                closeTag = "";
                z = 0;
            }
            else if(line[i] == '<' && line[i+1] != '/' && line[i+1] != '?' ) {
                tagPushed = false;
                openTag = "";
                j = 0;
            }
            if(closingtag) {
                closeTag += line[i];
                z++;
            }
            else if(!tagPushed){
                openTag += line[i];
                j++;
            }
            else if(!closingtag){
                if(line[i] == ' ' && text.size() == 0) continue;
                text += line[i];
            }


            if (openTag[0] == '<' && openTag[1] != '/' && openTag[j-1] == '>' && !tagPushed){
                if(openTag[1] != '?') stack.push(openTag.substr(1,j-2));
                stackempty = false;
                tagPushed = true;
                out << openTag.substr(0,j-1) << '>';
                out<<endl;
                openTag = "";
            }
            if (closeTag[0] == '<' && closeTag[1] == '/' && closeTag[z-1] == '>'){
                out << text ;
                text = "";
                if(stackempty){
                    out<<endl;
                    out << '<' << closeTag.substr(2,z-3) << '>';
                    out << '<' << '/' << closeTag.substr(2,z-3) << '>';
                    closeTag ="";
                    closingtag = false;
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
                        closeTag = "";
                        closingtag = false;
                    }
                    else
                    {
                        out << '<' << closeTag.substr(2,z-3) << '>';
                        out << '<' << '/' << closeTag.substr(2,z-3) << '>';
                    }
                    closeTag ="";
                    closingtag = false;
                    continue;
                } 
                else{
                    out << text << endl << closeTag <<endl;
                    text = "";
                    stack.pop();
                    stackempty = stack.empty();
                    closeTag ="";
                    closingtag = false;
                }
            }else if (i == line.length() -1) out << text;
        }
    }
}
void pretify(string input, string output){
    int depthfornextline = -1;
    ifstream in(input);
    ofstream out(output);
    string line;
    bool wastag = false;
    bool wasctag = false;
    while (getline(in,line))
    {
        string openTag;
        string closeTag;
        string text;
        bool tagPushed = true;
        bool closingtag = false;
        for (int i = 0, j = 0, z = 0; i < line.length(); i++)
        {
            if (line[i] == ' ' && (openTag.size() == 0 && text.size() == 0)){
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

            if (openTag[0] == '<' && openTag[1] != '/' && openTag[j-1] == '>' && !tagPushed){
                depthfornextline++;
                tagPushed = true;
                if(wastag == true  || wasctag == true) out<<endl;
                wastag = true;
                wasctag = false;
                for (int w = 0; w < depthfornextline; w++)
                {
                    out<<'\t';
                }
                out << openTag.substr(0,j-1) << '>';
                openTag = "";
                text = "";
            }
            if (closeTag[0] == '<' && closeTag[1] == '/' && closeTag[z-1] == '>'){
                if(text.size() != 0) out<<endl;
                for (int w = 0; w < depthfornextline+1; w++)
                {
                    out<<'\t';
                }
                wastag = false;
                wasctag = true;
                if(text.size() != 0){
                    out<< text;
                    text = "";           
                }
                depthfornextline--;
                if(wasctag || wastag) out<<endl;
                for (int w = 0; w < depthfornextline+1; w++)
                {
                    out<<'\t';
                }
                out << closeTag;
                closeTag = "";
                closingtag = false;

            }else if (i == line.length() -1){
                if(text.size() != 0) out<<endl;
                for (int w = 0; w < depthfornextline+1; w++)
                {
                    out<<'\t';
                }
                out << text;
                text = "";
            }
        } 
    }
}