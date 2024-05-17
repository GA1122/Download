BIGNUM *SRP_Calc_client_key(BIGNUM *N, BIGNUM *B, BIGNUM *g, BIGNUM *x, BIGNUM *a, BIGNUM *u)
	{
	BIGNUM *tmp = NULL, *tmp2 = NULL, *tmp3 = NULL , *k = NULL, *K = NULL;
	BN_CTX *bn_ctx;

	if (u == NULL || B == NULL || N == NULL || g == NULL || x == NULL || a == NULL ||
		(bn_ctx = BN_CTX_new()) == NULL)
		return NULL; 

	if ((tmp = BN_new()) == NULL ||
		(tmp2 = BN_new())== NULL ||
		(tmp3 = BN_new())== NULL ||
		(K = BN_new()) == NULL)
		goto err;
	
	if (!BN_mod_exp(tmp,g,x,N,bn_ctx))
		goto err;
	if (!(k = srp_Calc_k(N,g)))
		goto err;
	if (!BN_mod_mul(tmp2,tmp,k,N,bn_ctx))
		goto err;
	if (!BN_mod_sub(tmp,B,tmp2,N,bn_ctx))
		goto err;

	if (!BN_mod_mul(tmp3,u,x,N,bn_ctx))
		goto err;
	if (!BN_mod_add(tmp2,a,tmp3,N,bn_ctx))
		goto err;
	if (!BN_mod_exp(K,tmp,tmp2,N,bn_ctx))
		goto err;

err :
	BN_CTX_free(bn_ctx);
	BN_clear_free(tmp);
	BN_clear_free(tmp2);
	BN_clear_free(tmp3);
	BN_free(k);
	return K;	
	}
