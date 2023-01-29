#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define FIO ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);

struct user{
    int id;
    string name;
    vector<string>posts;
    vector<int>followers;
    user(int _id, string _name){id=_id, name=_name;}
};
string getMostInfluencer(vector<vector<int>>&adj, map<int, string>&antiID){
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
    map<int, string>antiID;
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
    cout<<"most influencer is "<<getMostInfluencer(adj, antiID)<<endl;
}
int main() {
#ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
#endif
    FIO
    int test = 1;
//    cin>>test;
    while (test--) SS();
    return 0;
}
