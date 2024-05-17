static void camellia_setup_tail(u64 *subkey, u64 *subRL, int max)
{
	u64 kw4, tt;
	u32 dw, tl, tr;

	 
	 
	subRL[3] ^= subRL[1];
	 
	subRL[5] ^= subRL[1];
	 
	subRL[7] ^= subRL[1];

	subRL[1] ^= (subRL[1] & ~subRL[9]) << 32;
	 
	dw = (subRL[1] & subRL[9]) >> 32;
	subRL[1] ^= rol32(dw, 1);

	 
	subRL[11] ^= subRL[1];
	 
	subRL[13] ^= subRL[1];
	 
	subRL[15] ^= subRL[1];

	subRL[1] ^= (subRL[1] & ~subRL[17]) << 32;
	 
	dw = (subRL[1] & subRL[17]) >> 32;
	subRL[1] ^= rol32(dw, 1);

	 
	subRL[19] ^= subRL[1];
	 
	subRL[21] ^= subRL[1];
	 
	subRL[23] ^= subRL[1];

	if (max == 24) {
		 
		subRL[24] ^= subRL[1];

		 
		kw4 = subRL[25];
	} else {
		subRL[1] ^= (subRL[1] & ~subRL[25]) << 32;
		 
		dw = (subRL[1] & subRL[25]) >> 32;
		subRL[1] ^= rol32(dw, 1);

		 
		subRL[27] ^= subRL[1];
		 
		subRL[29] ^= subRL[1];
		 
		subRL[31] ^= subRL[1];
		 
		subRL[32] ^= subRL[1];

		 
		kw4 = subRL[33];
		 
		subRL[30] ^= kw4;
		 
		subRL[28] ^= kw4;
		 
		subRL[26] ^= kw4;

		kw4 ^= (kw4 & ~subRL[24]) << 32;
		 
		dw = (kw4 & subRL[24]) >> 32;
		kw4 ^= rol32(dw, 1);
	}

	 
	subRL[22] ^= kw4;
	 
	subRL[20] ^= kw4;
	 
	subRL[18] ^= kw4;

	kw4 ^= (kw4 & ~subRL[16]) << 32;
	 
	dw = (kw4 & subRL[16]) >> 32;
	kw4 ^= rol32(dw, 1);

	 
	subRL[14] ^= kw4;
	 
	subRL[12] ^= kw4;
	 
	subRL[10] ^= kw4;

	kw4 ^= (kw4 & ~subRL[8]) << 32;
	 
	dw = (kw4 & subRL[8]) >> 32;
	kw4 ^= rol32(dw, 1);

	 
	subRL[6] ^= kw4;
	 
	subRL[4] ^= kw4;
	 
	subRL[2] ^= kw4;
	 
	subRL[0] ^= kw4;

	 
	SET_SUBKEY_LR(0, subRL[0] ^ subRL[2]);			 
	SET_SUBKEY_LR(2, subRL[3]);				 
	SET_SUBKEY_LR(3, subRL[2] ^ subRL[4]);			 
	SET_SUBKEY_LR(4, subRL[3] ^ subRL[5]);			 
	SET_SUBKEY_LR(5, subRL[4] ^ subRL[6]);			 
	SET_SUBKEY_LR(6, subRL[5] ^ subRL[7]);			 

	tl = (subRL[10] >> 32) ^ (subRL[10] & ~subRL[8]);
	dw = tl & (subRL[8] >> 32);				 
	tr = subRL[10] ^ rol32(dw, 1);
	tt = (tr | ((u64)tl << 32));

	SET_SUBKEY_LR(7, subRL[6] ^ tt);			 
	SET_SUBKEY_LR(8, subRL[8]);				 
	SET_SUBKEY_LR(9, subRL[9]);				 

	tl = (subRL[7] >> 32) ^ (subRL[7] & ~subRL[9]);
	dw = tl & (subRL[9] >> 32);				 
	tr = subRL[7] ^ rol32(dw, 1);
	tt = (tr | ((u64)tl << 32));

	SET_SUBKEY_LR(10, subRL[11] ^ tt);			 
	SET_SUBKEY_LR(11, subRL[10] ^ subRL[12]);		 
	SET_SUBKEY_LR(12, subRL[11] ^ subRL[13]);		 
	SET_SUBKEY_LR(13, subRL[12] ^ subRL[14]);		 
	SET_SUBKEY_LR(14, subRL[13] ^ subRL[15]);		 

	tl = (subRL[18] >> 32) ^ (subRL[18] & ~subRL[16]);
	dw = tl & (subRL[16] >> 32);				 
	tr = subRL[18] ^ rol32(dw, 1);
	tt = (tr | ((u64)tl << 32));

	SET_SUBKEY_LR(15, subRL[14] ^ tt);			 
	SET_SUBKEY_LR(16, subRL[16]);				 
	SET_SUBKEY_LR(17, subRL[17]);				 

	tl = (subRL[15] >> 32) ^ (subRL[15] & ~subRL[17]);
	dw = tl & (subRL[17] >> 32);				 
	tr = subRL[15] ^ rol32(dw, 1);
	tt = (tr | ((u64)tl << 32));

	SET_SUBKEY_LR(18, subRL[19] ^ tt);			 
	SET_SUBKEY_LR(19, subRL[18] ^ subRL[20]);		 
	SET_SUBKEY_LR(20, subRL[19] ^ subRL[21]);		 
	SET_SUBKEY_LR(21, subRL[20] ^ subRL[22]);		 
	SET_SUBKEY_LR(22, subRL[21] ^ subRL[23]);		 

	if (max == 24) {
		SET_SUBKEY_LR(23, subRL[22]);			 
		SET_SUBKEY_LR(24, subRL[24] ^ subRL[23]);	 
	} else {
		tl = (subRL[26] >> 32) ^ (subRL[26] & ~subRL[24]);
		dw = tl & (subRL[24] >> 32);			 
		tr = subRL[26] ^ rol32(dw, 1);
		tt = (tr | ((u64)tl << 32));

		SET_SUBKEY_LR(23, subRL[22] ^ tt);		 
		SET_SUBKEY_LR(24, subRL[24]);			 
		SET_SUBKEY_LR(25, subRL[25]);			 

		tl = (subRL[23] >> 32) ^ (subRL[23] & ~subRL[25]);
		dw = tl & (subRL[25] >> 32);			 
		tr = subRL[23] ^ rol32(dw, 1);
		tt = (tr | ((u64)tl << 32));

		SET_SUBKEY_LR(26, subRL[27] ^ tt);		 
		SET_SUBKEY_LR(27, subRL[26] ^ subRL[28]);	 
		SET_SUBKEY_LR(28, subRL[27] ^ subRL[29]);	 
		SET_SUBKEY_LR(29, subRL[28] ^ subRL[30]);	 
		SET_SUBKEY_LR(30, subRL[29] ^ subRL[31]);	 
		SET_SUBKEY_LR(31, subRL[30]);			 
		SET_SUBKEY_LR(32, subRL[32] ^ subRL[31]);	 
	}
}
