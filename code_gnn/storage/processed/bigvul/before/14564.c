BIGNUM *bn_dup_expand(const BIGNUM *b, int words)
	{
	BIGNUM *r = NULL;

	bn_check_top(b);

	 

	if (words > b->dmax)
		{
		BN_ULONG *a = bn_expand_internal(b, words);

		if (a)
			{
			r = BN_new();
			if (r)
				{
				r->top = b->top;
				r->dmax = words;
				r->neg = b->neg;
				r->d = a;
				}
			else
				{
				 
				OPENSSL_free(a);
				}
			}
		 
		}
	else
		{
		r = BN_dup(b);
		}

	bn_check_top(r);
	return r;
	}
