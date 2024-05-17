insert_scrambled_password(struct rad_handle *h, int srv)
{
	MD5_CTX ctx;
	unsigned char md5[16];
	const struct rad_server *srvp;
	int padded_len;
	int pos;

	srvp = &h->servers[srv];
	padded_len = h->pass_len == 0 ? 16 : (h->pass_len+15) & ~0xf;

	memcpy(md5, &h->request[POS_AUTH], LEN_AUTH);
	for (pos = 0;  pos < padded_len;  pos += 16) {
		int i;

		 
		MD5Init(&ctx);
		MD5Update(&ctx, srvp->secret, strlen(srvp->secret));
		MD5Update(&ctx, md5, 16);
		MD5Final(md5, &ctx);

		 
		for (i = 0;  i < 16;  i++)
			h->request[h->pass_pos + pos + i] =
			    md5[i] ^= h->pass[pos + i];
	}
}