int strzcmp16_h_n(const char16_t *s1H, size_t n1, const char16_t *s2N, size_t n2)
{
 const char16_t* e1 = s1H+n1;
 const char16_t* e2 = s2N+n2;

 while (s1H < e1 && s2N < e2) {
 const char16_t c2 = ntohs(*s2N);
 const int d = (int)*s1H++ - (int)c2;
        s2N++;
 if (d) {
 return d;
 }
 }

 return n1 < n2
 ? (0 - (int)ntohs(*s2N))
 : (n1 > n2
 ? ((int)*s1H - 0)

            : 0);
 }
