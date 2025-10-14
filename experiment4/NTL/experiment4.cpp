#include<iostream>
#include<NTL/ZZ.h>
#include<vector>
#include<queue>
using namespace std;
using namespace NTL;
void simple_try(ZZ & n,vector<ZZ>& result){
    //对n做素数分解，结果放在result里面,使用简单的试除法
    queue<ZZ> q;//临时队列
    q.push(n);
    while(!q.empty()){
        ZZ curr=q.front();
        q.pop();
        ZZ curr_sqrt=SqrRoot(curr);
        bool is_prime=true;//判断当前是否是素数
        if(curr%2==0){
            //先把当前数的所有因子2提取出来
            q.push(ZZ(2));
            q.push(curr/2);
            is_prime=false;
        }else{
            for(ZZ i(3);i<curr_sqrt;i+=2){
                //因子2已经提取完了，只需要提取奇因子即可
                if(curr%i==0){//找到因子
                    q.push(i);
                    q.push(curr/i);
                    is_prime=false;
                    break;
                }
            }
        }
        if(is_prime){
            result.push_back(curr);//当前是素数，放到结果中
        }
    }
}
int main(){
    ZZ i(210);//210=2*3*5*7
    vector<ZZ> result;
    // simple_try(i,result);
    cout<<"start"<<endl;
    // for(ZZ& p : result){
    //     cout<<p<<" ";
    // }
    cout<<"finish"<<endl;
    return 0;
}