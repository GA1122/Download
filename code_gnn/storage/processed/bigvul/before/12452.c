BIGNUM *SRP_Calc_A(BIGNUM *a, BIGNUM *N, BIGNUM *g)
	{
	BN_CTX *bn_ctx; 
	BIGNUM * A = NULL;

	if (a == NULL || N == NULL || g == NULL ||
		(bn_ctx = BN_CTX_new()) == NULL) 
		return NULL;

	if ((A = BN_new()) != NULL &&
	   !BN_mod_exp(A,g,a,N,bn_ctx))
		{
		BN_free(A);
		A = NULL;
		}
	BN_CTX_free(bn_ctx);
	return A;
	}
