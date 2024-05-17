static void camellia_setup256(const unsigned char *key, u32 *subkey)
{
	u32 kll, klr, krl, krr;         
	u32 krll, krlr, krrl, krrr;     
	u32 il, ir, t0, t1, w0, w1;     
	u32 subL[34];
	u32 subR[34];

	 
	kll = get_unaligned_be32(key);
	klr = get_unaligned_be32(key + 4);
	krl = get_unaligned_be32(key + 8);
	krr = get_unaligned_be32(key + 12);
	krll = get_unaligned_be32(key + 16);
	krlr = get_unaligned_be32(key + 20);
	krrl = get_unaligned_be32(key + 24);
	krrr = get_unaligned_be32(key + 28);

	 
	 
	subL[0] = kll; subR[0] = klr;
	 
	subL[1] = krl; subR[1] = krr;
	ROLDQo32(kll, klr, krl, krr, w0, w1, 45);
	 
	subL[12] = kll; subR[12] = klr;
	 
	subL[13] = krl; subR[13] = krr;
	ROLDQ(kll, klr, krl, krr, w0, w1, 15);
	 
	subL[16] = kll; subR[16] = klr;
	 
	subL[17] = krl; subR[17] = krr;
	ROLDQ(kll, klr, krl, krr, w0, w1, 17);
	 
	subL[22] = kll; subR[22] = klr;
	 
	subL[23] = krl; subR[23] = krr;
	ROLDQo32(kll, klr, krl, krr, w0, w1, 34);
	 
	subL[30] = kll; subR[30] = klr;
	 
	subL[31] = krl; subR[31] = krr;

	 
	ROLDQ(krll, krlr, krrl, krrr, w0, w1, 15);
	 
	subL[4] = krll; subR[4] = krlr;
	 
	subL[5] = krrl; subR[5] = krrr;
	ROLDQ(krll, krlr, krrl, krrr, w0, w1, 15);
	 
	subL[8] = krll; subR[8] = krlr;
	 
	subL[9] = krrl; subR[9] = krrr;
	ROLDQ(krll, krlr, krrl, krrr, w0, w1, 30);
	 
	subL[18] = krll; subR[18] = krlr;
	 
	subL[19] = krrl; subR[19] = krrr;
	ROLDQo32(krll, krlr, krrl, krrr, w0, w1, 34);
	 
	subL[26] = krll; subR[26] = krlr;
	 
	subL[27] = krrl; subR[27] = krrr;
	ROLDQo32(krll, krlr, krrl, krrr, w0, w1, 34);

	 
	kll = subL[0] ^ krll; klr = subR[0] ^ krlr;
	krl = subL[1] ^ krrl; krr = subR[1] ^ krrr;
	CAMELLIA_F(kll, klr,
		   CAMELLIA_SIGMA1L, CAMELLIA_SIGMA1R,
		   w0, w1, il, ir, t0, t1);
	krl ^= w0; krr ^= w1;
	CAMELLIA_F(krl, krr,
		   CAMELLIA_SIGMA2L, CAMELLIA_SIGMA2R,
		   kll, klr, il, ir, t0, t1);
	kll ^= krll; klr ^= krlr;
	CAMELLIA_F(kll, klr,
		   CAMELLIA_SIGMA3L, CAMELLIA_SIGMA3R,
		   krl, krr, il, ir, t0, t1);
	krl ^= w0 ^ krrl; krr ^= w1 ^ krrr;
	CAMELLIA_F(krl, krr,
		   CAMELLIA_SIGMA4L, CAMELLIA_SIGMA4R,
		   w0, w1, il, ir, t0, t1);
	kll ^= w0; klr ^= w1;

	 
	krll ^= kll; krlr ^= klr;
	krrl ^= krl; krrr ^= krr;
	CAMELLIA_F(krll, krlr,
		   CAMELLIA_SIGMA5L, CAMELLIA_SIGMA5R,
		   w0, w1, il, ir, t0, t1);
	krrl ^= w0; krrr ^= w1;
	CAMELLIA_F(krrl, krrr,
		   CAMELLIA_SIGMA6L, CAMELLIA_SIGMA6R,
		   w0, w1, il, ir, t0, t1);
	krll ^= w0; krlr ^= w1;

	 
	ROLDQ(kll, klr, krl, krr, w0, w1, 15);
	 
	subL[6] = kll; subR[6] = klr;
	 
	subL[7] = krl; subR[7] = krr;
	ROLDQ(kll, klr, krl, krr, w0, w1, 30);
	 
	subL[14] = kll; subR[14] = klr;
	 
	subL[15] = krl; subR[15] = krr;
	 
	 
	subL[24] = klr; subR[24] = krl;
	 
	subL[25] = krr; subR[25] = kll;
	 
	ROLDQo32(kll, klr, krl, krr, w0, w1, 49);
	 
	subL[28] = kll; subR[28] = klr;
	 
	subL[29] = krl; subR[29] = krr;

	 
	 
	subL[2] = krll; subR[2] = krlr;
	 
	subL[3] = krrl; subR[3] = krrr;
	ROLDQ(krll, krlr, krrl, krrr, w0, w1, 30);
	 
	subL[10] = krll; subR[10] = krlr;
	 
	subL[11] = krrl; subR[11] = krrr;
	ROLDQ(krll, krlr, krrl, krrr, w0, w1, 30);
	 
	subL[20] = krll; subR[20] = krlr;
	 
	subL[21] = krrl; subR[21] = krrr;
	ROLDQo32(krll, krlr, krrl, krrr, w0, w1, 51);
	 
	subL[32] = krll; subR[32] = krlr;
	 
	subL[33] = krrl; subR[33] = krrr;

	camellia_setup_tail(subkey, subL, subR, 32);
}
