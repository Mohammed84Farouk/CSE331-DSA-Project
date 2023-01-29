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

void SS(){
    int n;
    cin>>n;
    vector<user>v;
    for(int i=0;i<n;i++){
        int id;
        string s;
        cin>>id>>s;
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
        }
        v.push_back(u);
    }
    cout<<"size of vector of users is "<<v.size()<<endl;

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
