authentic_decode_pubkey_rsa(struct sc_context *ctx, unsigned char *blob, size_t blob_len,
		struct sc_pkcs15_prkey **out_key)
{
	struct sc_pkcs15_prkey_rsa *key;
	unsigned char *data;
	size_t data_len;
	int rv;

	LOG_FUNC_CALLED(ctx);

	if (!out_key)
		LOG_FUNC_RETURN(ctx, SC_ERROR_INVALID_ARGUMENTS);

	if (!(*out_key))   {
		*out_key = calloc(1, sizeof(struct sc_pkcs15_prkey));

		if (!(*out_key))
			LOG_TEST_RET(ctx, SC_ERROR_OUT_OF_MEMORY, "Cannot callocate pkcs15 private key");

		(*out_key)->algorithm = SC_ALGORITHM_RSA;
	}
	else if (*out_key && (*out_key)->algorithm != SC_ALGORITHM_RSA)   {
		LOG_FUNC_RETURN(ctx, SC_ERROR_INVALID_DATA);
	}

	key = &(*out_key)->u.rsa;

	rv = authentic_get_tagged_data(ctx, blob, blob_len, AUTHENTIC_TAG_RSA_PUBLIC, &data, &data_len);
	LOG_TEST_RET(ctx, rv, "cannot get public key SDO data");

	blob = data;
	blob_len = data_len;

	 
	rv = authentic_get_tagged_data(ctx, blob, blob_len, AUTHENTIC_TAG_RSA_PUBLIC_MODULUS, &data, &data_len);
	LOG_TEST_RET(ctx, rv, "cannot get public key SDO data");

	if (key->modulus.data)
		free(key->modulus.data);
	key->modulus.data = calloc(1, data_len);
	if (!key->modulus.data)
		LOG_TEST_RET(ctx, SC_ERROR_OUT_OF_MEMORY, "Cannot callocate modulus BN");
	memcpy(key->modulus.data, data, data_len);
	key->modulus.len = data_len;

	 
	rv = authentic_get_tagged_data(ctx, blob, blob_len, AUTHENTIC_TAG_RSA_PUBLIC_EXPONENT, &data, &data_len);
	LOG_TEST_RET(ctx, rv, "cannot get public key SDO data");

	if (key->exponent.data)
		free(key->exponent.data);
	key->exponent.data = calloc(1, data_len);
	if (!key->exponent.data)
		LOG_TEST_RET(ctx, SC_ERROR_OUT_OF_MEMORY, "Cannot callocate modulus BN");
	memcpy(key->exponent.data, data, data_len);
	key->exponent.len = data_len;

	LOG_FUNC_RETURN(ctx, rv);
}