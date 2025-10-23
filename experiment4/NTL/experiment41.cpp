#include<iostream>
#include<NTL/ZZ.h>
#include<vector>
#include<queue>
#include<algorithm>
#include<map>
#include<fstream>
#include<string>
#include<chrono>
using namespace std;
using namespace NTL;
using namespace chrono;

map<ZZ,int> factor_sortAndStd(vector<ZZ>& factor){
    //对整理好的factor做排序和标准素因子分解
    sort(factor.begin(),factor.end());
    map<ZZ,int> factor_std;//map储存因子factor，键位对应素因子，值为次数
    for (const ZZ & p:factor){
        factor_std[p]++;
    }
    return factor_std;
}
void simple_try(ZZ &n,vector<ZZ>& factor,map<ZZ,int>& factor_std){
    if(n==1||n==2){
        factor.push_back(n);
        return;
    }//排除简单情况
    queue<ZZ> temp;
    temp.push(n);
    while(!temp.empty()){
        //从队列中取数并出队
        ZZ curr = temp.front();
        temp.pop();
        bool is_prime = true;
        if (curr == 2){
            //遇到2直接放入因子结果中
            factor.push_back(ZZ(2));
            is_prime = false;
        }else if(curr % 2 == 0){
            //先把因子2全部拿掉
            temp.push(ZZ(2));
            temp.push(curr/2);
            is_prime = false;
        }else{
            ZZ curr_sqrt = SqrRoot(curr);
            for (ZZ i(3) ;i <= curr_sqrt;i+=2) {
                if(curr % i == 0){
                    temp.push(i);
                    temp.push(curr/i);
                    is_prime = false;
                    break;

                }
            }
        }
        if (is_prime){
            factor.push_back(curr);
        }
    }
    factor_std = factor_sortAndStd(factor);
}
int main(){

    ifstream file("附件.txt");
    string line;
    auto start = high_resolution_clock::now();
    while(getline(file,line)){
        if(line.empty()) continue;
        ZZ n(INIT_VAL,line.c_str());
        vector<ZZ> factor;
        map<ZZ,int> factor_std;
        simple_try(n,factor,factor_std);
        //输出结果n=p_1*p_2*...=p_1^alpha_1 p_2^alpha2...的形式
        cout<<n<<"=";
        for(size_t i =0;i<factor.size();i++){
            cout<<factor[i];
            if(i != factor.size()-1){
                cout<<"*";//最后一个元素后面不加*号
            }
        }
        cout<<"=";
        auto it = factor_std.begin();
        while(it != factor_std.end()){
            cout<<it->first<<"^"<<it->second;
            it++;
            if(it!=factor_std.end()){
                cout<<"*";
            }
        }
        cout<<endl;
    }
    auto end = high_resolution_clock::now();//结束时间
    auto duration = duration_cast<milliseconds>(end - start);
    cout<<"用时:"<<duration.count()<<"ms"<<endl;
    file.close();
    return 0;
   

}