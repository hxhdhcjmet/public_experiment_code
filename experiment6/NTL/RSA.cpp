#include<iostream>
#include<NTL/ZZ.h>
#include<vector>
#include<chrono>
using namespace std;
using namespace NTL;
using namespace chrono;
const size_t randmlen = 1;//测试密文个数
vector<ZZ> generateargs(long bits){
    //生成两个1024bit p,q,指定e = 65537,计算n = p*q,d = e^-1 mod varphi(n)
    ZZ p,q;
    GenPrime(p,bits);
    GenPrime(q,bits);
    while(p==q){
    GenPrime(q,bits);//生成两个不同的素数
    }
    ZZ n = p*q;
    ZZ varphi = (p-1)*(q-1);
    ZZ e(65537);
    while(GCD(e,varphi)!=1){
    //如果e与varphi不同余，则再计算
    e+=2;
    }

    //计算d,即e模varphi的逆元
    ZZ d = InvMod(e,varphi);
    //计算RSA优化用到的d_p = d mod p-1,d_q = d mod q-1
    ZZ d_p = d%(p-1);
    ZZ d_q = d%(q-1);
    ZZ p_q = InvMod(p%q,q);
    ZZ q_p = InvMod(q%p,p);
    vector<ZZ> args = {e,d,n,p,q,d_p,d_q,p_q,q_p};
    return args;
}

ZZ RSAencode(const ZZ &m,const ZZ &e,const ZZ &n){
    //m为明文，(c,n)为公开公钥
    return PowerMod(m,e,n);
}
ZZ RSAdecode(const ZZ& c,const ZZ &d,const ZZ &n){
    //c为密文，(d,n)为私钥
    return PowerMod(c,d,n);
}

ZZ crt_RSAdecode(const ZZ& c,const ZZ &p,const ZZ &q,const ZZ &d_p,const ZZ &d_q,
    const ZZ &q_p){
    /*利用中国剩余定理优化RSA的解密
    m = mp = c^d_p mod p
    m = mq = c^d_q mod q
    令m=mq+k*q,带入一号式子得到kq = mp-mq mod p,
    k = (mp-mq)*q_p mod p
    处理mp-mq mod p且为正数,得到k,带入得到
    m = mq+k*q
    */
   ZZ m_p = PowerMod(c%p,d_p,p);
   ZZ m_q = PowerMod(c%q,d_q,q);

   ZZ diff = (m_p - m_q)%p;
   if (diff < 0) diff+=p;

   ZZ h = (diff*q_p)%p;
   return m_q+h*q;
}

void authorize(const ZZ& m,const ZZ& m1){
    //验证原明文m和解密后的明文m1是否相等，打印提示信息
    if (m == m1){
    cout<<":) RSA decrypt right!"<<endl;
    return;
    }
    cout<<":( RSA decrypt wrong!"<<endl;
}
// 中国剩余定理优化RSA解密,加密过程相同
int main(){
    //设置随机数种子
    ZZ seed;
    time_t t;
    time(&t);
    seed = t;
    SetSeed(seed);

    //设置p,q长度为1024比特
    long bits = 1024;
    
    //计算参数并计算时间
    cout<<"start to generate arguments p,q,n,e,d..."<<endl;
    auto start = high_resolution_clock::now();
    vector<ZZ> args = generateargs(bits);
    auto end = high_resolution_clock::now();
    auto genTime = duration_cast<microseconds>(end - start).count();
    cout<<"generate the arguments using time:"<<genTime<<"μs"<<endl;
    // 提取参数并打印
    ZZ e = args[0],d = args[1],n = args[2],p = args[3],q = args[4],d_p = args[5],
    d_q = args[6],p_q = args[7],q_p = args[8];
    cout<<"e:"<<e<<endl<<"d:"<<d<<endl<<"n:"<<n<<endl<<"p:"<<p<<endl<<"q:"<<q<<endl<<
    "d_p:"<<d_p<<endl<<"d_q:"<<d_q<<endl<<"p^{-1}:"<<p_q<<endl<<"q^{-1}:"<<q_p<<endl;
    cout<<"======================================================================"<<endl;
    cout<<"start to test RSA..."<<endl;
    for(size_t i = 0;i<randmlen;i++){
        ZZ m = RandomBnd(n);//生成0-n-1范围内的随机数
        cout<<"======================================================================"<<endl;
        cout<<"plaintext:"<<m<<endl;
        start = high_resolution_clock::now();
        cout<<"RSA encrypting..."<<endl;
        ZZ encryption = RSAencode(m,e,n);
        end = high_resolution_clock::now();
        auto usingtime = duration_cast<microseconds>(end - start).count();
        cout<<"ciphertext:"<<encryption<<endl;
        cout<<"using time:"<<usingtime<<"μs"<<endl;

        //一般RSA解密
        cout<<"======================================================================"<<endl;
        cout<<"RSA decrypting..."<<endl;
        start = high_resolution_clock::now();
        ZZ decryption = RSAdecode(encryption,d,n);
        end = high_resolution_clock::now();
        usingtime = duration_cast<microseconds>(end - start).count();
        cout<<"decrypt result:"<<decryption<<endl;
        authorize(m,decryption);
        cout<<"using time:"<<usingtime<<"μs"<<endl; 

        //CRT优化RSA解密
        cout<<"======================================================================"<<endl;
        cout<<"CRT optimized RSA decrypting..."<<endl;
        start = high_resolution_clock::now();
        ZZ decryption_crt = crt_RSAdecode(encryption,p,q,d_p,d_q,q_p);
        end = high_resolution_clock::now();
        usingtime = duration_cast<microseconds>(end - start).count();
        cout<<"CRT optimized decrypt result:"<<decryption_crt<<endl;
        authorize(m,decryption_crt);
        cout<<"using time:"<<usingtime<<"μs"<<endl;
    
    }
    return 0;
    

}