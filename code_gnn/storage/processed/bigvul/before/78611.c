pgp_store_key(sc_card_t *card, sc_cardctl_openpgp_keystore_info_t *key_info)
{
	sc_context_t *ctx = card->ctx;
	sc_cardctl_openpgp_keygen_info_t pubkey;
	u8 *data = NULL;
	size_t len = 0;
	int r;

	LOG_FUNC_CALLED(ctx);

	 
	if (key_info->keytype < 1 || key_info->keytype > 3) {
		sc_log(ctx, "Unknown key type %d.", key_info->keytype);
		LOG_FUNC_RETURN(ctx, SC_ERROR_INVALID_ARGUMENTS);
	}
	 
	switch (key_info->keyformat) {
	case SC_OPENPGP_KEYFORMAT_STD:
	case SC_OPENPGP_KEYFORMAT_STDN:
		break;

	case SC_OPENPGP_KEYFORMAT_CRT:
	case SC_OPENPGP_KEYFORMAT_CRTN:
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_NOT_SUPPORTED);

	default:
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_INVALID_ARGUMENTS);
	}

	 
	if (key_info->e_len > 4) {
		sc_log(card->ctx,
		       "Exponent %"SC_FORMAT_LEN_SIZE_T"u-bit (>32) is not supported.",
		       key_info->e_len * 8);
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_NOT_SUPPORTED);
	}

	 
	memset(&pubkey, 0, sizeof(pubkey));
	pubkey.keytype = key_info->keytype;
	if (key_info->n && key_info->n_len) {
		pubkey.modulus = key_info->n;
		pubkey.modulus_len = 8*key_info->n_len;
		 
	}
	r = pgp_update_new_algo_attr(card, &pubkey);
	LOG_TEST_RET(card->ctx, r, "Failed to update new algorithm attributes");
	 
	r = pgp_build_extended_header_list(card, key_info, &data, &len);
	if (r < 0) {
		sc_log(ctx, "Failed to build Extended Header list.");
		goto out;
	}
	 
	r = pgp_put_data(card, 0x4D, data, len);
	if (r < 0) {
		sc_log(ctx, "Failed to write to DO.");
		goto out;
	}

	free(data);
	data = NULL;

	 
	r = pgp_store_creationtime(card, key_info->keytype, &key_info->creationtime);
	LOG_TEST_RET(card->ctx, r, "Cannot store creation time");

	 
	sc_log(card->ctx, "Calculate and store fingerprint");
	r = pgp_calculate_and_store_fingerprint(card, key_info->creationtime, key_info->n, key_info->e, &pubkey);
	LOG_TEST_RET(card->ctx, r, "Cannot store fingerprint.");
	 
	sc_log(card->ctx, "Update blobs holding pubkey info.");
	r = pgp_update_pubkey_blob(card, key_info->n, 8*key_info->n_len,
	                           key_info->e, 8*key_info->e_len, key_info->keytype);

	sc_log(ctx, "Update card algorithms.");
	pgp_update_card_algorithms(card, &pubkey);

out:
	if (data) {
		free(data);
		data = NULL;
	}
	LOG_FUNC_RETURN(ctx, r);
}