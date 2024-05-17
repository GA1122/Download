static int tls1_PRF(long digest_mask,
		     const void *seed1, int seed1_len,
		     const void *seed2, int seed2_len,
		     const void *seed3, int seed3_len,
		     const void *seed4, int seed4_len,
		     const void *seed5, int seed5_len,
		     const unsigned char *sec, int slen,
		     unsigned char *out1,
		     unsigned char *out2, int olen)
	{
	int len,i,idx,count;
	const unsigned char *S1;
	long m;
	const EVP_MD *md;
	int ret = 0;

	 
	count=0;
	for (idx=0;ssl_get_handshake_digest(idx,&m,&md);idx++) {
		if ((m<<TLS1_PRF_DGST_SHIFT) & digest_mask) count++;
	}	
	len=slen/count;
	if (count == 1)
		slen = 0;
	S1=sec;
	memset(out1,0,olen);
	for (idx=0;ssl_get_handshake_digest(idx,&m,&md);idx++) {
		if ((m<<TLS1_PRF_DGST_SHIFT) & digest_mask) {
			if (!md) {
				SSLerr(SSL_F_TLS1_PRF,
				SSL_R_UNSUPPORTED_DIGEST_TYPE);
				goto err;				
			}
			if (!tls1_P_hash(md ,S1,len+(slen&1),
					seed1,seed1_len,seed2,seed2_len,seed3,seed3_len,seed4,seed4_len,seed5,seed5_len,
					out2,olen))
				goto err;
			S1+=len;
			for (i=0; i<olen; i++)
			{
				out1[i]^=out2[i];
			}
		}
	}
	ret = 1;
err:
	return ret;
}
