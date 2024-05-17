pgp_update_pubkey_blob(sc_card_t *card, u8* modulus, size_t modulus_len,
                       u8* exponent, size_t exponent_len, u8 key_id)
{
	struct pgp_priv_data *priv = DRVDATA(card);
	pgp_blob_t *pk_blob;
	unsigned int blob_id;
	sc_pkcs15_pubkey_t pubkey;
	u8 *data = NULL;
	size_t len;
	int r;

	LOG_FUNC_CALLED(card->ctx);

	if (key_id == SC_OPENPGP_KEY_SIGN)
		blob_id = DO_SIGN_SYM;
	else if (key_id == SC_OPENPGP_KEY_ENCR)
		blob_id = DO_ENCR_SYM;
	else if (key_id == SC_OPENPGP_KEY_AUTH)
		blob_id = DO_AUTH_SYM;
	else {
		sc_log(card->ctx, "Unknown key id %X.", key_id);
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_INVALID_ARGUMENTS);
	}

	sc_log(card->ctx, "Get the blob %X.", blob_id);
	r = pgp_get_blob(card, priv->mf, blob_id, &pk_blob);
	LOG_TEST_RET(card->ctx, r, "Cannot get the blob.");

	 
	memset(&pubkey, 0, sizeof(pubkey));
	pubkey.algorithm = SC_ALGORITHM_RSA;
	pubkey.u.rsa.modulus.data  = modulus;
	pubkey.u.rsa.modulus.len   = modulus_len >> 3;   
	pubkey.u.rsa.exponent.data = exponent;
	pubkey.u.rsa.exponent.len  = exponent_len >> 3;

	r = sc_pkcs15_encode_pubkey(card->ctx, &pubkey, &data, &len);
	LOG_TEST_RET(card->ctx, r, "Cannot encode pubkey.");

	sc_log(card->ctx, "Update blob content.");
	r = pgp_set_blob(pk_blob, data, len);
	LOG_TEST_RET(card->ctx, r, "Cannot update blob content.");
	LOG_FUNC_RETURN(card->ctx, r);
}