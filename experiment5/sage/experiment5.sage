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
