static void compute_bitreverse(int n, uint16 *rev)
{
   int ld = ilog(n) - 1;  
   int i, n8 = n >> 3;
   for (i=0; i < n8; ++i)
      rev[i] = (bit_reverse(i) >> (32-ld+3)) << 2;
}