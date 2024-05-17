static void camellia_do_encrypt(const u32 *subkey, u32 *io, unsigned max)
{
	u32 il, ir, t0, t1;             

	 
	io[0] ^= SUBKEY_L(0);
	io[1] ^= SUBKEY_R(0);

	 
#define ROUNDS(i) ({ \
	CAMELLIA_ROUNDSM(io[0], io[1], \
			 SUBKEY_L(i + 2), SUBKEY_R(i + 2), \
			 io[2], io[3], il, ir); \
	CAMELLIA_ROUNDSM(io[2], io[3], \
			 SUBKEY_L(i + 3), SUBKEY_R(i + 3), \
			 io[0], io[1], il, ir); \
	CAMELLIA_ROUNDSM(io[0], io[1], \
			 SUBKEY_L(i + 4), SUBKEY_R(i + 4), \
			 io[2], io[3], il, ir); \
	CAMELLIA_ROUNDSM(io[2], io[3], \
			 SUBKEY_L(i + 5), SUBKEY_R(i + 5), \
			 io[0], io[1], il, ir); \
	CAMELLIA_ROUNDSM(io[0], io[1], \
			 SUBKEY_L(i + 6), SUBKEY_R(i + 6), \
			 io[2], io[3], il, ir); \
	CAMELLIA_ROUNDSM(io[2], io[3], \
			 SUBKEY_L(i + 7), SUBKEY_R(i + 7), \
			 io[0], io[1], il, ir); \
})
#define FLS(i) ({ \
	CAMELLIA_FLS(io[0], io[1], io[2], io[3], \
		     SUBKEY_L(i + 0), SUBKEY_R(i + 0), \
		     SUBKEY_L(i + 1), SUBKEY_R(i + 1), \
		     t0, t1, il, ir); \
})

	ROUNDS(0);
	FLS(8);
	ROUNDS(8);
	FLS(16);
	ROUNDS(16);
	if (max == 32) {
		FLS(24);
		ROUNDS(24);
	}

#undef ROUNDS
#undef FLS

	 
	io[2] ^= SUBKEY_L(max);
	io[3] ^= SUBKEY_R(max);
	 
}
