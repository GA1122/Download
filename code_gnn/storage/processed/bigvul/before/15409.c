static int tls1_P_hash(const EVP_MD *md, const unsigned char *sec,
			int sec_len,
			const void *seed1, int seed1_len,
			const void *seed2, int seed2_len,
			const void *seed3, int seed3_len,
			const void *seed4, int seed4_len,
			const void *seed5, int seed5_len,
			unsigned char *out, int olen)
	{
	int chunk;
	size_t j;
	EVP_MD_CTX ctx, ctx_tmp;
	EVP_PKEY *mac_key;
	unsigned char A1[EVP_MAX_MD_SIZE];
	size_t A1_len;
	int ret = 0;
	
	chunk=EVP_MD_size(md);
	OPENSSL_assert(chunk >= 0);

	EVP_MD_CTX_init(&ctx);
	EVP_MD_CTX_init(&ctx_tmp);
	EVP_MD_CTX_set_flags(&ctx, EVP_MD_CTX_FLAG_NON_FIPS_ALLOW);
	EVP_MD_CTX_set_flags(&ctx_tmp, EVP_MD_CTX_FLAG_NON_FIPS_ALLOW);
	mac_key = EVP_PKEY_new_mac_key(EVP_PKEY_HMAC, NULL, sec, sec_len);
	if (!mac_key)
		goto err;
	if (!EVP_DigestSignInit(&ctx,NULL,md, NULL, mac_key))
		goto err;
	if (!EVP_DigestSignInit(&ctx_tmp,NULL,md, NULL, mac_key))
		goto err;
	if (seed1 && !EVP_DigestSignUpdate(&ctx,seed1,seed1_len))
		goto err;
	if (seed2 && !EVP_DigestSignUpdate(&ctx,seed2,seed2_len))
		goto err;
	if (seed3 && !EVP_DigestSignUpdate(&ctx,seed3,seed3_len))
		goto err;
	if (seed4 && !EVP_DigestSignUpdate(&ctx,seed4,seed4_len))
		goto err;
	if (seed5 && !EVP_DigestSignUpdate(&ctx,seed5,seed5_len))
		goto err;
	if (!EVP_DigestSignFinal(&ctx,A1,&A1_len))
		goto err;

	for (;;)
		{
		 
		if (!EVP_DigestSignInit(&ctx,NULL,md, NULL, mac_key))
			goto err;
		if (!EVP_DigestSignInit(&ctx_tmp,NULL,md, NULL, mac_key))
			goto err;
		if (!EVP_DigestSignUpdate(&ctx,A1,A1_len))
			goto err;
		if (!EVP_DigestSignUpdate(&ctx_tmp,A1,A1_len))
			goto err;
		if (seed1 && !EVP_DigestSignUpdate(&ctx,seed1,seed1_len))
			goto err;
		if (seed2 && !EVP_DigestSignUpdate(&ctx,seed2,seed2_len))
			goto err;
		if (seed3 && !EVP_DigestSignUpdate(&ctx,seed3,seed3_len))
			goto err;
		if (seed4 && !EVP_DigestSignUpdate(&ctx,seed4,seed4_len))
			goto err;
		if (seed5 && !EVP_DigestSignUpdate(&ctx,seed5,seed5_len))
			goto err;

		if (olen > chunk)
			{
			if (!EVP_DigestSignFinal(&ctx,out,&j))
				goto err;
			out+=j;
			olen-=j;
			 
			if (!EVP_DigestSignFinal(&ctx_tmp,A1,&A1_len))
				goto err;
			}
		else	 
			{
			if (!EVP_DigestSignFinal(&ctx,A1,&A1_len))
				goto err;
			memcpy(out,A1,olen);
			break;
			}
		}
	ret = 1;
err:
	EVP_PKEY_free(mac_key);
	EVP_MD_CTX_cleanup(&ctx);
	EVP_MD_CTX_cleanup(&ctx_tmp);
	OPENSSL_cleanse(A1,sizeof(A1));
	return ret;
	}
