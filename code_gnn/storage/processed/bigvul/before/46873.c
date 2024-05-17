static void camellia_setup192(const unsigned char *key, u64 *subkey)
{
	unsigned char kk[32];
	u64 krl, krr;

	memcpy(kk, key, 24);
	memcpy((unsigned char *)&krl, key+16, 8);
	krr = ~krl;
	memcpy(kk+24, (unsigned char *)&krr, 8);
	camellia_setup256(kk, subkey);
}
