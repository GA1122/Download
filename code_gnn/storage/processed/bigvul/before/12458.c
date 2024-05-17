int SRP_Verify_B_mod_N(BIGNUM *B, BIGNUM *N)
	{
	BIGNUM *r;
	BN_CTX *bn_ctx; 
	int ret = 0;

	if (B == NULL || N == NULL ||
		(bn_ctx = BN_CTX_new()) == NULL)
		return 0;

	if ((r = BN_new()) == NULL)
		goto err;
	 
	if (!BN_nnmod(r,B,N,bn_ctx))
		goto err;
	ret = !BN_is_zero(r);
err:
	BN_CTX_free(bn_ctx);
	BN_free(r);
	return ret;
	}
