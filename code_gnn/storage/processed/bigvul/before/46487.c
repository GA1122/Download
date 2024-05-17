void bn_sqr_words(BN_ULONG *r, const BN_ULONG *a, int n)
        {
	if (n <= 0) return;

	while (n&~3)
		{
		sqr(r[0],r[1],a[0]);
		sqr(r[2],r[3],a[1]);
		sqr(r[4],r[5],a[2]);
		sqr(r[6],r[7],a[3]);
		a+=4; r+=8; n-=4;
		}
	if (n)
		{
		sqr(r[0],r[1],a[0]); if (--n == 0) return;
		sqr(r[2],r[3],a[1]); if (--n == 0) return;
		sqr(r[4],r[5],a[2]);
		}
	}
