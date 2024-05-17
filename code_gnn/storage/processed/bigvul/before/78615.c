pgp_update_new_algo_attr(sc_card_t *card, sc_cardctl_openpgp_keygen_info_t *key_info)
{
	struct pgp_priv_data *priv = DRVDATA(card);
	pgp_blob_t *algo_blob;
	unsigned int old_modulus_len;      
	unsigned int old_exponent_len;
	const unsigned int tag = 0x00C0 | key_info->keytype;
	u8 changed = 0;
	int r = SC_SUCCESS;

	LOG_FUNC_CALLED(card->ctx);
	 
	r = pgp_seek_blob(card, priv->mf, (0x00C0 | key_info->keytype), &algo_blob);
	LOG_TEST_RET(card->ctx, r, "Cannot get old algorithm attributes");
	old_modulus_len = bebytes2ushort(algo_blob->data + 1);   
	sc_log(card->ctx,
	       "Old modulus length %d, new %"SC_FORMAT_LEN_SIZE_T"u.",
	       old_modulus_len, key_info->modulus_len);
	old_exponent_len = bebytes2ushort(algo_blob->data + 3);   
	sc_log(card->ctx,
	       "Old exponent length %d, new %"SC_FORMAT_LEN_SIZE_T"u.",
	       old_exponent_len, key_info->exponent_len);

	 
	 
	if (key_info->modulus_len == 0) {
		sc_log(card->ctx, "Use old modulus length (%d).", old_modulus_len);
		key_info->modulus_len = old_modulus_len;
	}
	 
	else if (old_modulus_len != key_info->modulus_len) {
		algo_blob->data[1] = (unsigned char)(key_info->modulus_len >> 8);
		algo_blob->data[2] = (unsigned char)key_info->modulus_len;
		changed = 1;
	}

	 
	if (key_info->exponent_len == 0) {
		sc_log(card->ctx, "Use old exponent length (%d).", old_exponent_len);
		key_info->exponent_len = old_exponent_len;
	}
	else if (old_exponent_len != key_info->exponent_len) {
		algo_blob->data[3] = (unsigned char)(key_info->exponent_len >> 8);
		algo_blob->data[4] = (unsigned char)key_info->exponent_len;
		changed = 1;
	}

	 
	if (changed) {
		r = pgp_put_data(card, tag, algo_blob->data, 6);
		 
		LOG_TEST_RET(card->ctx, r, "Cannot set new algorithm attributes");
	}

	LOG_FUNC_RETURN(card->ctx, r);
}
