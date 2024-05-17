static void camellia_setup128(const unsigned char *key, u32 *subkey)
{
	u32 kll, klr, krl, krr;
	u32 il, ir, t0, t1, w0, w1;
	u32 subL[26];
	u32 subR[26];

	 
	kll = get_unaligned_be32(key);
	klr = get_unaligned_be32(key + 4);
	krl = get_unaligned_be32(key + 8);
	krr = get_unaligned_be32(key + 12);

	 
	 
	subL[0] = kll; subR[0] = klr;
	 
	subL[1] = krl; subR[1] = krr;
	 
	ROLDQ(kll, klr, krl, krr, w0, w1, 15);
	 
	subL[4] = kll; subR[4] = klr;
	 
	subL[5] = krl; subR[5] = krr;
	 
	ROLDQ(kll, klr, krl, krr, w0, w1, 30);
	 
	subL[10] = kll; subR[10] = klr;
	 
	subL[11] = krl; subR[11] = krr;
	 
	ROLDQ(kll, klr, krl, krr, w0, w1, 15);
	 
	subL[13] = krl; subR[13] = krr;
	 
	ROLDQ(kll, klr, krl, krr, w0, w1, 17);
	 
	subL[16] = kll; subR[16] = klr;
	 
	subL[17] = krl; subR[17] = krr;
	 
	ROLDQ(kll, klr, krl, krr, w0, w1, 17);
	 
	subL[18] = kll; subR[18] = klr;
	 
	subL[19] = krl; subR[19] = krr;
	 
	ROLDQ(kll, klr, krl, krr, w0, w1, 17);
	 
	subL[22] = kll; subR[22] = klr;
	 
	subL[23] = krl; subR[23] = krr;

	 
	kll = subL[0]; klr = subR[0];
	krl = subL[1]; krr = subR[1];
	CAMELLIA_F(kll, klr,
		   CAMELLIA_SIGMA1L, CAMELLIA_SIGMA1R,
		   w0, w1, il, ir, t0, t1);
	krl ^= w0; krr ^= w1;
	CAMELLIA_F(krl, krr,
		   CAMELLIA_SIGMA2L, CAMELLIA_SIGMA2R,
		   kll, klr, il, ir, t0, t1);
	 
	CAMELLIA_F(kll, klr,
		   CAMELLIA_SIGMA3L, CAMELLIA_SIGMA3R,
		   krl, krr, il, ir, t0, t1);
	krl ^= w0; krr ^= w1;
	CAMELLIA_F(krl, krr,
		   CAMELLIA_SIGMA4L, CAMELLIA_SIGMA4R,
		   w0, w1, il, ir, t0, t1);
	kll ^= w0; klr ^= w1;

	 
	 
	subL[2] = kll; subR[2] = klr;
	subL[3] = krl; subR[3] = krr;
	ROLDQ(kll, klr, krl, krr, w0, w1, 15);
	 
	subL[6] = kll; subR[6] = klr;
	subL[7] = krl; subR[7] = krr;
	ROLDQ(kll, klr, krl, krr, w0, w1, 15);
	 
	subL[8] = kll; subR[8] = klr;
	subL[9] = krl; subR[9] = krr;
	ROLDQ(kll, klr, krl, krr, w0, w1, 15);
	 
	subL[12] = kll; subR[12] = klr;
	ROLDQ(kll, klr, krl, krr, w0, w1, 15);
	 
	subL[14] = kll; subR[14] = klr;
	subL[15] = krl; subR[15] = krr;
	ROLDQo32(kll, klr, krl, krr, w0, w1, 34);
	 
	subL[20] = kll; subR[20] = klr;
	subL[21] = krl; subR[21] = krr;
	ROLDQ(kll, klr, krl, krr, w0, w1, 17);
	 
	subL[24] = kll; subR[24] = klr;
	subL[25] = krl; subR[25] = krr;

	camellia_setup_tail(subkey, subL, subR, 24);
}
