int test_kron(BIO *bp, BN_CTX *ctx)
	{
	BN_GENCB cb;
	BIGNUM *a,*b,*r,*t;
	int i;
	int legendre, kronecker;
	int ret = 0;

	a = BN_new();
	b = BN_new();
	r = BN_new();
	t = BN_new();
	if (a == NULL || b == NULL || r == NULL || t == NULL) goto err;

	BN_GENCB_set(&cb, genprime_cb, NULL);
	
	 

	if (!BN_generate_prime_ex(b, 512, 0, NULL, NULL, &cb)) goto err;
	b->neg = rand_neg();
	putc('\n', stderr);

	for (i = 0; i < num0; i++)
		{
		if (!BN_bntest_rand(a, 512, 0, 0)) goto err;
		a->neg = rand_neg();

		 
		if (!BN_copy(t, b)) goto err;
		t->neg = 0;
		if (!BN_sub_word(t, 1)) goto err;
		if (!BN_rshift1(t, t)) goto err;
		 
		b->neg=0;
		
		if (!BN_mod_exp_recp(r, a, t, b, ctx)) goto err;
		b->neg=1;

		if (BN_is_word(r, 1))
			legendre = 1;
		else if (BN_is_zero(r))
			legendre = 0;
		else
			{
			if (!BN_add_word(r, 1)) goto err;
			if (0 != BN_ucmp(r, b))
				{
				fprintf(stderr, "Legendre symbol computation failed\n");
				goto err;
				}
			legendre = -1;
			}
		
		kronecker = BN_kronecker(a, b, ctx);
		if (kronecker < -1) goto err;
		 
		if (a->neg && b->neg)
			kronecker = -kronecker;
		
		if (legendre != kronecker)
			{
			fprintf(stderr, "legendre != kronecker; a = ");
			BN_print_fp(stderr, a);
			fprintf(stderr, ", b = ");
			BN_print_fp(stderr, b);
			fprintf(stderr, "\n");
			goto err;
			}

		putc('.', stderr);
		fflush(stderr);
		}

	putc('\n', stderr);
	fflush(stderr);
	ret = 1;
 err:
	if (a != NULL) BN_free(a);
	if (b != NULL) BN_free(b);
	if (r != NULL) BN_free(r);
	if (t != NULL) BN_free(t);
	return ret;
	}
