static void camellia_setup_tail(u32 *subkey, u32 *subL, u32 *subR, int max)
{
	u32 dw, tl, tr;
	u32 kw4l, kw4r;

	 
	 
	subL[3] ^= subL[1]; subR[3] ^= subR[1];
	 
	subL[5] ^= subL[1]; subR[5] ^= subR[1];
	 
	subL[7] ^= subL[1]; subR[7] ^= subR[1];
	subL[1] ^= subR[1] & ~subR[9];
	dw = subL[1] & subL[9];
	subR[1] ^= rol32(dw, 1);  
	 
	subL[11] ^= subL[1]; subR[11] ^= subR[1];
	 
	subL[13] ^= subL[1]; subR[13] ^= subR[1];
	 
	subL[15] ^= subL[1]; subR[15] ^= subR[1];
	subL[1] ^= subR[1] & ~subR[17];
	dw = subL[1] & subL[17];
	subR[1] ^= rol32(dw, 1);  
	 
	subL[19] ^= subL[1]; subR[19] ^= subR[1];
	 
	subL[21] ^= subL[1]; subR[21] ^= subR[1];
	 
	subL[23] ^= subL[1]; subR[23] ^= subR[1];
	if (max == 24) {
		 
		subL[24] ^= subL[1]; subR[24] ^= subR[1];

	 
		kw4l = subL[25]; kw4r = subR[25];
	} else {
		subL[1] ^= subR[1] & ~subR[25];
		dw = subL[1] & subL[25];
		subR[1] ^= rol32(dw, 1);  
		 
		subL[27] ^= subL[1]; subR[27] ^= subR[1];
		 
		subL[29] ^= subL[1]; subR[29] ^= subR[1];
		 
		subL[31] ^= subL[1]; subR[31] ^= subR[1];
		 
		subL[32] ^= subL[1]; subR[32] ^= subR[1];

	 
		kw4l = subL[33]; kw4r = subR[33];
		 
		subL[30] ^= kw4l; subR[30] ^= kw4r;
		 
		subL[28] ^= kw4l; subR[28] ^= kw4r;
		 
		subL[26] ^= kw4l; subR[26] ^= kw4r;
		kw4l ^= kw4r & ~subR[24];
		dw = kw4l & subL[24];
		kw4r ^= rol32(dw, 1);  
	}
	 
	subL[22] ^= kw4l; subR[22] ^= kw4r;
	 
	subL[20] ^= kw4l; subR[20] ^= kw4r;
	 
	subL[18] ^= kw4l; subR[18] ^= kw4r;
	kw4l ^= kw4r & ~subR[16];
	dw = kw4l & subL[16];
	kw4r ^= rol32(dw, 1);  
	 
	subL[14] ^= kw4l; subR[14] ^= kw4r;
	 
	subL[12] ^= kw4l; subR[12] ^= kw4r;
	 
	subL[10] ^= kw4l; subR[10] ^= kw4r;
	kw4l ^= kw4r & ~subR[8];
	dw = kw4l & subL[8];
	kw4r ^= rol32(dw, 1);  
	 
	subL[6] ^= kw4l; subR[6] ^= kw4r;
	 
	subL[4] ^= kw4l; subR[4] ^= kw4r;
	 
	subL[2] ^= kw4l; subR[2] ^= kw4r;
	 
	subL[0] ^= kw4l; subR[0] ^= kw4r;

	 
	SUBKEY_L(0) = subL[0] ^ subL[2]; 
	SUBKEY_R(0) = subR[0] ^ subR[2];
	SUBKEY_L(2) = subL[3];        
	SUBKEY_R(2) = subR[3];
	SUBKEY_L(3) = subL[2] ^ subL[4];  
	SUBKEY_R(3) = subR[2] ^ subR[4];
	SUBKEY_L(4) = subL[3] ^ subL[5];  
	SUBKEY_R(4) = subR[3] ^ subR[5];
	SUBKEY_L(5) = subL[4] ^ subL[6];  
	SUBKEY_R(5) = subR[4] ^ subR[6];
	SUBKEY_L(6) = subL[5] ^ subL[7];  
	SUBKEY_R(6) = subR[5] ^ subR[7];
	tl = subL[10] ^ (subR[10] & ~subR[8]);
	dw = tl & subL[8];   
	tr = subR[10] ^ rol32(dw, 1);
	SUBKEY_L(7) = subL[6] ^ tl;  
	SUBKEY_R(7) = subR[6] ^ tr;
	SUBKEY_L(8) = subL[8];        
	SUBKEY_R(8) = subR[8];
	SUBKEY_L(9) = subL[9];        
	SUBKEY_R(9) = subR[9];
	tl = subL[7] ^ (subR[7] & ~subR[9]);
	dw = tl & subL[9];   
	tr = subR[7] ^ rol32(dw, 1);
	SUBKEY_L(10) = tl ^ subL[11];  
	SUBKEY_R(10) = tr ^ subR[11];
	SUBKEY_L(11) = subL[10] ^ subL[12];  
	SUBKEY_R(11) = subR[10] ^ subR[12];
	SUBKEY_L(12) = subL[11] ^ subL[13];  
	SUBKEY_R(12) = subR[11] ^ subR[13];
	SUBKEY_L(13) = subL[12] ^ subL[14];  
	SUBKEY_R(13) = subR[12] ^ subR[14];
	SUBKEY_L(14) = subL[13] ^ subL[15];  
	SUBKEY_R(14) = subR[13] ^ subR[15];
	tl = subL[18] ^ (subR[18] & ~subR[16]);
	dw = tl & subL[16];  
	tr = subR[18] ^ rol32(dw, 1);
	SUBKEY_L(15) = subL[14] ^ tl;  
	SUBKEY_R(15) = subR[14] ^ tr;
	SUBKEY_L(16) = subL[16];      
	SUBKEY_R(16) = subR[16];
	SUBKEY_L(17) = subL[17];      
	SUBKEY_R(17) = subR[17];
	tl = subL[15] ^ (subR[15] & ~subR[17]);
	dw = tl & subL[17];  
	tr = subR[15] ^ rol32(dw, 1);
	SUBKEY_L(18) = tl ^ subL[19];  
	SUBKEY_R(18) = tr ^ subR[19];
	SUBKEY_L(19) = subL[18] ^ subL[20];  
	SUBKEY_R(19) = subR[18] ^ subR[20];
	SUBKEY_L(20) = subL[19] ^ subL[21];  
	SUBKEY_R(20) = subR[19] ^ subR[21];
	SUBKEY_L(21) = subL[20] ^ subL[22];  
	SUBKEY_R(21) = subR[20] ^ subR[22];
	SUBKEY_L(22) = subL[21] ^ subL[23];  
	SUBKEY_R(22) = subR[21] ^ subR[23];
	if (max == 24) {
		SUBKEY_L(23) = subL[22];      
		SUBKEY_R(23) = subR[22];
		SUBKEY_L(24) = subL[24] ^ subL[23];  
		SUBKEY_R(24) = subR[24] ^ subR[23];
	} else {
		tl = subL[26] ^ (subR[26] & ~subR[24]);
		dw = tl & subL[24];  
		tr = subR[26] ^ rol32(dw, 1);
		SUBKEY_L(23) = subL[22] ^ tl;  
		SUBKEY_R(23) = subR[22] ^ tr;
		SUBKEY_L(24) = subL[24];      
		SUBKEY_R(24) = subR[24];
		SUBKEY_L(25) = subL[25];      
		SUBKEY_R(25) = subR[25];
		tl = subL[23] ^ (subR[23] & ~subR[25]);
		dw = tl & subL[25];  
		tr = subR[23] ^ rol32(dw, 1);
		SUBKEY_L(26) = tl ^ subL[27];  
		SUBKEY_R(26) = tr ^ subR[27];
		SUBKEY_L(27) = subL[26] ^ subL[28];  
		SUBKEY_R(27) = subR[26] ^ subR[28];
		SUBKEY_L(28) = subL[27] ^ subL[29];  
		SUBKEY_R(28) = subR[27] ^ subR[29];
		SUBKEY_L(29) = subL[28] ^ subL[30];  
		SUBKEY_R(29) = subR[28] ^ subR[30];
		SUBKEY_L(30) = subL[29] ^ subL[31];  
		SUBKEY_R(30) = subR[29] ^ subR[31];
		SUBKEY_L(31) = subL[30];      
		SUBKEY_R(31) = subR[30];
		SUBKEY_L(32) = subL[32] ^ subL[31];  
		SUBKEY_R(32) = subR[32] ^ subR[31];
	}
}
