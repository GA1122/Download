int strzcmp16(const char16_t *s1, size_t n1, const char16_t *s2, size_t n2)
{
 const char16_t* e1 = s1+n1;
 const char16_t* e2 = s2+n2;

 while (s1 < e1 && s2 < e2) {
 const int d = (int)*s1++ - (int)*s2++;
 if (d) {
 return d;
 }
 }

 return n1 < n2
 ? (0 - (int)*s2)
 : (n1 > n2
 ? ((int)*s1 - 0)
 : 0);
}
