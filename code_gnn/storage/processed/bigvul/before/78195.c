authentic_manage_sdo_encode_prvkey(struct sc_card *card, struct sc_pkcs15_prkey *prvkey,
			unsigned char **out, size_t *out_len)
{
	struct sc_context *ctx = card->ctx;
	struct sc_pkcs15_prkey_rsa rsa;
	unsigned char *blob = NULL, *blob01 = NULL;
	size_t blob_len = 0, blob01_len = 0;
	int rv;

	if (!prvkey || !out || !out_len)
		LOG_TEST_RET(ctx, SC_ERROR_INVALID_ARGUMENTS, "Invalid arguments");
	if (prvkey->algorithm != SC_ALGORITHM_RSA)
		LOG_TEST_RET(ctx, SC_ERROR_INVALID_DATA, "Invalid SDO operation");

	rsa = prvkey->u.rsa;
	 
	rv = authentic_update_blob(ctx, AUTHENTIC_TAG_RSA_PRIVATE_P, rsa.p.data, rsa.p.len, &blob, &blob_len);
	LOG_TEST_RET(ctx, rv, "SDO RSA P encode error");

	rv = authentic_update_blob(ctx, AUTHENTIC_TAG_RSA_PRIVATE_Q, rsa.q.data, rsa.q.len, &blob, &blob_len);
	LOG_TEST_RET(ctx, rv, "SDO RSA Q encode error");

	rv = authentic_update_blob(ctx, AUTHENTIC_TAG_RSA_PRIVATE_PQ, rsa.iqmp.data, rsa.iqmp.len, &blob, &blob_len);
	LOG_TEST_RET(ctx, rv, "SDO RSA PQ encode error");

	rv = authentic_update_blob(ctx, AUTHENTIC_TAG_RSA_PRIVATE_DP1, rsa.dmp1.data, rsa.dmp1.len, &blob, &blob_len);
	LOG_TEST_RET(ctx, rv, "SDO RSA DP1 encode error");

	rv = authentic_update_blob(ctx, AUTHENTIC_TAG_RSA_PRIVATE_DQ1, rsa.dmq1.data, rsa.dmq1.len, &blob, &blob_len);
	LOG_TEST_RET(ctx, rv, "SDO RSA DQ1 encode error");

	rv = authentic_update_blob(ctx, AUTHENTIC_TAG_RSA_PRIVATE, blob, blob_len, &blob01, &blob01_len);
	LOG_TEST_RET(ctx, rv, "SDO RSA Private encode error");

	free (blob);
	blob = NULL;
	blob_len = 0;

	 
	sc_log(ctx,
	       "modulus.len:%"SC_FORMAT_LEN_SIZE_T"u blob_len:%"SC_FORMAT_LEN_SIZE_T"u",
	       rsa.modulus.len, blob_len);
	rv = authentic_update_blob(ctx, AUTHENTIC_TAG_RSA_PUBLIC_MODULUS, rsa.modulus.data, rsa.modulus.len, &blob, &blob_len);
	LOG_TEST_RET(ctx, rv, "SDO RSA Modulus encode error");

	sc_log(ctx,
	       "exponent.len:%"SC_FORMAT_LEN_SIZE_T"u blob_len:%"SC_FORMAT_LEN_SIZE_T"u",
	       rsa.exponent.len, blob_len);
	rv = authentic_update_blob(ctx, AUTHENTIC_TAG_RSA_PUBLIC_EXPONENT, rsa.exponent.data, rsa.exponent.len, &blob, &blob_len);
	LOG_TEST_RET(ctx, rv, "SDO RSA Exponent encode error");

	rv = authentic_update_blob(ctx, AUTHENTIC_TAG_RSA_PUBLIC, blob, blob_len, &blob01, &blob01_len);
	LOG_TEST_RET(ctx, rv, "SDO RSA Private encode error");

	free (blob);

	rv = authentic_update_blob(ctx, AUTHENTIC_TAG_RSA, blob01, blob01_len, out, out_len);
	LOG_TEST_RET(ctx, rv, "SDO RSA encode error");

	free(blob01);

	LOG_FUNC_RETURN(ctx, rv);
}