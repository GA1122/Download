BIGNUM *SRP_Calc_x(BIGNUM *s, const char *user, const char *pass)
	{
	unsigned char dig[SHA_DIGEST_LENGTH];
	EVP_MD_CTX ctxt;
	unsigned char *cs;

	if ((s == NULL) ||
		(user == NULL) ||
		(pass == NULL))
		return NULL;

	if ((cs = OPENSSL_malloc(BN_num_bytes(s))) == NULL)
		return NULL;

	EVP_MD_CTX_init(&ctxt);
	EVP_DigestInit_ex(&ctxt, EVP_sha1(), NULL);
	EVP_DigestUpdate(&ctxt, user, strlen(user));
	EVP_DigestUpdate(&ctxt, ":", 1);
	EVP_DigestUpdate(&ctxt, pass, strlen(pass));
	EVP_DigestFinal_ex(&ctxt, dig, NULL);

	EVP_DigestInit_ex(&ctxt, EVP_sha1(), NULL);
	BN_bn2bin(s,cs);
	EVP_DigestUpdate(&ctxt, cs, BN_num_bytes(s));
	OPENSSL_free(cs);
	EVP_DigestUpdate(&ctxt, dig, sizeof(dig));
	EVP_DigestFinal_ex(&ctxt, dig, NULL);
	EVP_MD_CTX_cleanup(&ctxt);

	return BN_bin2bn(dig, sizeof(dig), NULL);
	}
