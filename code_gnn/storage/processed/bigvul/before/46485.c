BN_ULONG bn_mul_add_words(BN_ULONG *rp, const BN_ULONG *ap, int num, BN_ULONG w)
	{
	BN_ULONG c1=0;

	if (num <= 0) return(c1);

	while (num&~3)
		{
		mul_add(rp[0],ap[0],w,c1);
		mul_add(rp[1],ap[1],w,c1);
		mul_add(rp[2],ap[2],w,c1);
		mul_add(rp[3],ap[3],w,c1);
		ap+=4; rp+=4; num-=4;
		}
	if (num)
		{
		mul_add(rp[0],ap[0],w,c1); if (--num==0) return c1;
		mul_add(rp[1],ap[1],w,c1); if (--num==0) return c1;
		mul_add(rp[2],ap[2],w,c1); return c1;
		}
	
	return(c1);
	} 
