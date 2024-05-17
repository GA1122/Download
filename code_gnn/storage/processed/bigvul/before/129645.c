inline void doMultiply(const AffineTransform& t1, const AffineTransform& t2, AffineTransform* res)
{
    res->setA(t1.a() * t2.a() + t1.c() * t2.b());
    res->setB(t1.b() * t2.a() + t1.d() * t2.b());
    res->setC(t1.a() * t2.c() + t1.c() * t2.d());
    res->setD(t1.b() * t2.c() + t1.d() * t2.d());
    res->setE(t1.a() * t2.e() + t1.c() * t2.f() + t1.e());
    res->setF(t1.b() * t2.e() + t1.d() * t2.f() + t1.f());
}
