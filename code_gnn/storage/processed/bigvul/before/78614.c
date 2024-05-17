pgp_update_card_algorithms(sc_card_t *card, sc_cardctl_openpgp_keygen_info_t *key_info)
{
	sc_algorithm_info_t *algo;
	u8 id = key_info->keytype;

	LOG_FUNC_CALLED(card->ctx);

	if (id > card->algorithm_count) {
		sc_log(card->ctx,
		       "This key ID %u is out of the card's algorithm list.",
		       (unsigned int)id);
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_INVALID_ARGUMENTS);
	}

	 
	algo = card->algorithms + (id - 1);
	 
	algo->key_length = (unsigned int)key_info->modulus_len;
	LOG_FUNC_RETURN(card->ctx, SC_SUCCESS);
}