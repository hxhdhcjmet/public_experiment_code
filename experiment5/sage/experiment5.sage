from tqdm import *
def bsgs(g, y, p):
    m = int(ceil(sqrt(p - 1)))
    S = {pow(g, j, p): j for j in range(m)}
    gs = pow(g, p - 1 - m, p)
    for i in range(m):
        if y in S:
            return i * m + S[y]
        y = y * gs % p
    return None
def pohlig_hellman(p, g,h, factors):
    res=[]
    mod=[]
    order=p-1
    for pi,e in factors:
        g_i= pow(g, order//(pi^e),p)
        hi= pow(h, order//(pi^e),p)
        xi= bsgs(g_i,hi,p)
        res.append(xi)
        mod.append(pi^e)
    return crt(res, mod)
def pollard_rho(g,h,p):
    n = p -1
    c = randrange(1, n)
    def f(X, a, b):
        if X % 3 == 0:
            return (X * g) % p, (a + 1) % n, b
        elif X % 3 == 1:
            return (X * h) % p, a, (b + 1) % n
        else:
            return X*X % p, (2 * a) % n, (2 * b) % n
    x, a, b = 1, 0, 0
    y, a1, b1 = x, a, b 
    while 1:
        x,a,b = f(x,a,b)
        y,a1,b1 = f(y,a1,b1)
        y,a1,b1 = f(y,a1,b1)
        if x == y and b != b1:
            break
    B = (b1-b)%n
    A = (a-a1)%n
    d = gcd(B,n)
    r = (A//d)*inverse_mod(B//d,n//d) % (n//d)
    print("Find Collision")
    for i in range(d):
        r += (n//d)
        if pow(g,r,p) == h:
            return r
test =[(768075001,17,750713277),[866704609,53,210331466],[916636177,5,481985755],[652580214507899,2,77240405458667],[827370290159669027, 2, 282875805618794488]]
# for i in range(3):
#     p,g,h= test[i]
#     x= pohlig_hellman(p,g,h,list(factor(p-1)))
#     print(f"{g}^x= {h} (mod {p})，解得x={x}")
for i in range(5):
    p,g,h= test[i]
    %time x= pollard_rho(g,h,p)
    print(f"{g}^x= {h} (mod {p})，解得x={x}")
##
Find Collision
CPU times: user 286 ms, sys: 19.5 ms, total: 306 ms
Wall time: 298 ms
17^x= 750713277 (mod 768075001)，解得x=731948411
Find Collision
CPU times: user 623 ms, sys: 8.11 ms, total: 631 ms
Wall time: 630 ms
53^x= 210331466 (mod 866704609)，解得x=653693977
Find Collision
CPU times: user 195 ms, sys: 4.62 ms, total: 200 ms
Wall time: 200 ms
5^x= 481985755 (mod 916636177)，解得x=745897096
Find Collision
CPU times: user 2min 33s, sys: 0 ns, total: 2min 33s
Wall time: 2min 33s
2^x= 77240405458667 (mod 652580214507899)，解得x=472601834152789
Find Collision
CPU times: user 5h 4min 36s, sys: 3min 54s, total: 5h 8min 31s
Wall time: 5h 8min 30s
2^x= 282875805618794488 (mod 827370290159669027)，解得x=642261215921134456
##
