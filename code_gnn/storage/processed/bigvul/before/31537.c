rad_demangle_mppe_key(struct rad_handle *h, const void *mangled, size_t mlen, u_char *demangled, size_t *len)
{
	char R[LEN_AUTH];     
	const char *S;
	u_char b[16];
	const u_char *A, *C;
	MD5_CTX Context;
	int Slen, i, Clen, Ppos;
	u_char *P;

	if (mlen % 16 != SALT_LEN) {
		generr(h, "Cannot interpret mangled data of length %ld", (u_long)mlen);
		return -1;
	}

	 
	if (rad_request_authenticator(h, R, sizeof R) != LEN_AUTH) {
		generr(h, "Cannot obtain the RADIUS request authenticator");
		return -1;
	}

	A = (const u_char *)mangled;       
	C = (const u_char *)mangled + SALT_LEN;   
	Clen = mlen - SALT_LEN;
	S = rad_server_secret(h);     
	Slen = strlen(S);
	P = alloca(Clen);         

	MD5Init(&Context);
	MD5Update(&Context, S, Slen);
	MD5Update(&Context, R, LEN_AUTH);
	MD5Update(&Context, A, SALT_LEN);
	MD5Final(b, &Context);
	Ppos = 0;

	while (Clen) {
		Clen -= 16;

		for (i = 0; i < 16; i++)
		    P[Ppos++] = C[i] ^ b[i];

		if (Clen) {
			MD5Init(&Context);
			MD5Update(&Context, S, Slen);
			MD5Update(&Context, C, 16);
			MD5Final(b, &Context);
		}
                
		C += 16;
	}

	 
	*len = *P;
	if (*len > mlen - 1) {
		generr(h, "Mangled data seems to be garbage %d %d", *len, mlen-1);        
		return -1;
	}

	if (*len > MPPE_KEY_LEN) {
		generr(h, "Key to long (%d) for me max. %d", *len, MPPE_KEY_LEN);        
		return -1;
	}

	memcpy(demangled, P + 1, *len);
	return 0;
}
