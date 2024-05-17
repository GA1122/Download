rad_demangle(struct rad_handle *h, const void *mangled, size_t mlen, u_char *demangled) 
{
	char R[LEN_AUTH];
	const char *S;
	int i, Ppos;
	MD5_CTX Context;
	u_char b[16], *C;

	if ((mlen % 16 != 0) || (mlen > 128)) {
		generr(h, "Cannot interpret mangled data of length %ld", (u_long)mlen);
		return -1;
	}

	C = (u_char *)mangled;

	 
	S = rad_server_secret(h);

	 
	if (rad_request_authenticator(h, R, sizeof R) != LEN_AUTH) {
		generr(h, "Cannot obtain the RADIUS request authenticator");
                return -1;
	}

	MD5Init(&Context);
	MD5Update(&Context, S, strlen(S));
	MD5Update(&Context, R, LEN_AUTH);
	MD5Final(b, &Context);
	Ppos = 0;
	while (mlen) {

		mlen -= 16;
		for (i = 0; i < 16; i++)
			demangled[Ppos++] = C[i] ^ b[i];

		if (mlen) {
			MD5Init(&Context);
			MD5Update(&Context, S, strlen(S));
			MD5Update(&Context, C, 16);
			MD5Final(b, &Context);
		}

		C += 16;
	}

	return 0;
}