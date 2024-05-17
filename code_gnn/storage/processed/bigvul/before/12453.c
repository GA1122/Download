BIGNUM *SRP_Calc_B(BIGNUM *b, BIGNUM *N, BIGNUM *g, BIGNUM *v)
	{
	BIGNUM  *kv = NULL, *gb = NULL;
	BIGNUM *B = NULL, *k = NULL;
	BN_CTX *bn_ctx;

	if (b == NULL || N == NULL || g == NULL || v == NULL ||
		(bn_ctx = BN_CTX_new()) == NULL)
		return NULL; 

	if ( (kv = BN_new()) == NULL ||
		(gb = BN_new()) == NULL ||
		(B = BN_new())== NULL)
		goto err;

	 

	if (!BN_mod_exp(gb,g,b,N,bn_ctx) ||
	   !(k = srp_Calc_k(N,g)) ||
	   !BN_mod_mul(kv,v,k,N,bn_ctx) || 
	   !BN_mod_add(B,gb,kv,N,bn_ctx))
		{
		BN_free(B);
		B = NULL;
		}
err:
	BN_CTX_free(bn_ctx);
	BN_clear_free(kv);
	BN_clear_free(gb);
	BN_free(k); 
	return B;
	}
