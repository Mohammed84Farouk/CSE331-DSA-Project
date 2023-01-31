#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define FIO ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);

struct user{
    int id;
    string name;
    vector<string>posts, topics;
    vector<int>followers;
    user(int _id, string _name){id=_id, name=_name;}
};
string getMostInfluencer(vector<vector<int>>&adj, vector<string>&antiID){
    int mx=0, influencer=-1;
    for(int i=1;i<=adj.size();i++)
        if(adj[i].size()>mx)
            mx=adj[i].size(), influencer=i;
    return antiID[influencer];
}
void SS(){
    int n;
    cin>>n;
    vector<user>v;
    vector<vector<int>>adj(n+1);
    vector<string>antiID(2e6);                      // Maximum number of IDs I can get so far will be 2000000
    for(int i=0;i<n;i++){
        int id;
        string s;
        cin>>id>>s;
        antiID[id]=s;
        user u(id, s);
        int p;
        cin>>p;
        cin.ignore();
        while(p--){
            string post;
            getline(cin, post);
            u.posts.push_back(post);
        }
        int f;
        cin>>f;
        while(f--){
            int follower;
            cin>>follower;
            u.followers.push_back(follower);
            adj[id].push_back(f);                   // f follows id
        }
        v.push_back(u);
    }
    cout<<"The most Influencer is "<<getMostInfluencer(adj, antiID)<<endl;
}
//f2
//returns vector of closing and opening tags
vector <string> tags(string minifiedString){
    string tagName;
    vector <string> tagsNames;
    bool openTheTag = false;
    int stringSize = minifiedString.size();
    for (int i = 0; i < stringSize ; i++) {
        if(minifiedString[i]=='<'){
            openTheTag = true;
            continue;
        }
        if (openTheTag){
            if(minifiedString[i]=='>'){
                openTheTag = false;
                tagsNames.push_back(tagName);
                tagName = "";
                continue;
            }
            else{
                tagName += minifiedString[i];
            }
        }
    }
    return tagsNames;
}

//f3
//gets the data of each tag
vector<string> tagsData(string minifiedString){
    vector<string> DataOfTags;
    int minifiedStringSize = minifiedString.size();
    string temp;
    for(int i = 0; i < minifiedStringSize; i++){
        if(minifiedString[i] != '/'){
            temp += minifiedString[i];
        }
        else{
            while(minifiedString[i] != '>'){
                i++;
            }
            if(i+1 < minifiedStringSize)
                temp += minifiedString[i+1];
        }
    }
    int tempSize = temp.size();
    for (int i = 0; i < tempSize; i++) {
        string data;
        if((temp[i]=='>') && (i+1 < tempSize)){
            if(temp[i+1] == '<'){
                DataOfTags.push_back("null");
            } else{
                for (int j = i+1; temp[j] != '<'; j++) {
                    data += temp[j];
                }
                DataOfTags.push_back(data);
            }
        }
    }
    return DataOfTags;
}
int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
#endif
    FIO
    string s = "<users><user><id>1</id><name>Ahmed Ali</name><posts><post><body> Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. </body><topics><topic> economy </topic><topic> finance </topic></topics></post><post><body> Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. </body><topics><topic> solar_energy </topic></topics></post></posts><followers><follower><id>2</id></follower><follower><id>3</id></follower></followers></user><user><id>2</id><name>Yasser Ahmed</name><posts><post><body> Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. </body><topics><topic> education </topic></topics></post></posts><followers><follower><id>1</id></follower></followers></user><user><id>3</id><name>Mohamed Sherif</name><posts><post><body> Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. </body><topics><topic> sports </topic></topics></post></posts><followers><follower><id>1</id></follower></followers></user></users>";
    int test = 1;
//    cin>>test;
    while (test--) SS();
    return 0;
}
