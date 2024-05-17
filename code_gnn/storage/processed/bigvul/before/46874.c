static void camellia_setup256(const unsigned char *key, u64 *subkey)
{
	u64 kl, kr;			 
	u64 krl, krr;			 
	u64 ww;				 
	u64 subRL[34];

	 
	kl = get_unaligned_be64(key);
	kr = get_unaligned_be64(key + 8);
	krl = get_unaligned_be64(key + 16);
	krr = get_unaligned_be64(key + 24);

	 
	 
	subRL[0] = kl;
	 
	subRL[1] = kr;
	ROLDQ(kl, kr, 45);
	 
	subRL[12] = kl;
	 
	subRL[13] = kr;
	ROLDQ(kl, kr, 15);
	 
	subRL[16] = kl;
	 
	subRL[17] = kr;
	ROLDQ(kl, kr, 17);
	 
	subRL[22] = kl;
	 
	subRL[23] = kr;
	ROLDQ(kl, kr, 34);
	 
	subRL[30] = kl;
	 
	subRL[31] = kr;

	 
	ROLDQ(krl, krr, 15);
	 
	subRL[4] = krl;
	 
	subRL[5] = krr;
	ROLDQ(krl, krr, 15);
	 
	subRL[8] = krl;
	 
	subRL[9] = krr;
	ROLDQ(krl, krr, 30);
	 
	subRL[18] = krl;
	 
	subRL[19] = krr;
	ROLDQ(krl, krr, 34);
	 
	subRL[26] = krl;
	 
	subRL[27] = krr;
	ROLDQ(krl, krr, 34);

	 
	kl = subRL[0] ^ krl;
	kr = subRL[1] ^ krr;

	CAMELLIA_F(kl, CAMELLIA_SIGMA1L, CAMELLIA_SIGMA1R, ww);
	kr ^= ww;
	CAMELLIA_F(kr, CAMELLIA_SIGMA2L, CAMELLIA_SIGMA2R, kl);
	kl ^= krl;
	CAMELLIA_F(kl, CAMELLIA_SIGMA3L, CAMELLIA_SIGMA3R, kr);
	kr ^= ww ^ krr;
	CAMELLIA_F(kr, CAMELLIA_SIGMA4L, CAMELLIA_SIGMA4R, ww);
	kl ^= ww;

	 
	krl ^= kl;
	krr ^= kr;
	CAMELLIA_F(krl, CAMELLIA_SIGMA5L, CAMELLIA_SIGMA5R, ww);
	krr ^= ww;
	CAMELLIA_F(krr, CAMELLIA_SIGMA6L, CAMELLIA_SIGMA6R, ww);
	krl ^= ww;

	 
	ROLDQ(kl, kr, 15);
	 
	subRL[6] = kl;
	 
	subRL[7] = kr;
	ROLDQ(kl, kr, 30);
	 
	subRL[14] = kl;
	 
	subRL[15] = kr;
	 
	ROLDQ(kl, kr, 32);
	 
	subRL[24] = kl;
	 
	subRL[25] = kr;
	 
	ROLDQ(kl, kr, 17);
	 
	subRL[28] = kl;
	 
	subRL[29] = kr;

	 
	 
	subRL[2] = krl;
	 
	subRL[3] = krr;
	ROLDQ(krl, krr, 30);
	 
	subRL[10] = krl;
	 
	subRL[11] = krr;
	ROLDQ(krl, krr, 30);
	 
	subRL[20] = krl;
	 
	subRL[21] = krr;
	ROLDQ(krl, krr, 51);
	 
	subRL[32] = krl;
	 
	subRL[33] = krr;

	camellia_setup_tail(subkey, subRL, 32);
}
