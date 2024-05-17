int test_small_prime(BIO *bp,BN_CTX *ctx)
	{
	static const int bits = 10;
	int ret = 0;
	BIGNUM r;

	BN_init(&r);
	if (!BN_generate_prime_ex(&r, bits, 0, NULL, NULL, NULL))
		goto err;
	if (BN_num_bits(&r) != bits)
		{
		BIO_printf(bp, "Expected %d bit prime, got %d bit number\n", bits, BN_num_bits(&r));
		goto err;
		}

	ret = 1;

err:
	BN_clear(&r);
	return ret;
	}
