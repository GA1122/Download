pgp_gen_key(sc_card_t *card, sc_cardctl_openpgp_keygen_info_t *key_info)
{
	sc_apdu_t apdu;
	 
	u8 apdu_case;
	u8 *apdu_data;
	size_t apdu_le;
	size_t resplen = 0;
	int r = SC_SUCCESS;

	LOG_FUNC_CALLED(card->ctx);

	 
	 
	if (key_info->keytype == SC_OPENPGP_KEY_SIGN)
		apdu_data = (unsigned char *) "\xb6";
		 
	else if (key_info->keytype == SC_OPENPGP_KEY_ENCR)
		apdu_data = (unsigned char *) "\xb8";
	else if (key_info->keytype == SC_OPENPGP_KEY_AUTH)
		apdu_data = (unsigned char *) "\xa4";
	else {
		sc_log(card->ctx, "Unknown key type %X.", key_info->keytype);
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_INVALID_ARGUMENTS);
	}

	if (card->type == SC_CARD_TYPE_OPENPGP_GNUK && key_info->modulus_len != 2048) {
		sc_log(card->ctx, "Gnuk does not support other key length than 2048.");
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_INVALID_ARGUMENTS);
	}

	 
	r = pgp_update_new_algo_attr(card, key_info);
	LOG_TEST_RET(card->ctx, r, "Cannot set attributes for new-generated key");

	 
	if (card->caps & SC_CARD_CAP_APDU_EXT
		&& key_info->modulus_len > 1900
		&& card->type != SC_CARD_TYPE_OPENPGP_GNUK) {
		 
		apdu_le = card->max_recv_size;
		apdu_case = SC_APDU_CASE_4_EXT;
	}
	else {
		apdu_case = SC_APDU_CASE_4_SHORT;
		apdu_le = 256;
		resplen = MAXLEN_RESP_PUBKEY;
	}
	if (card->type == SC_CARD_TYPE_OPENPGP_GNUK) {
		resplen = MAXLEN_RESP_PUBKEY_GNUK;
	}

	 
	sc_format_apdu(card, &apdu, apdu_case, 0x47, 0x80, 0);
	apdu.data = apdu_data;
	apdu.datalen = 2;   
	apdu.lc = 2;
	apdu.le = apdu_le;

	 
	apdu.resplen = (resplen > 0) ? resplen : apdu_le;
	apdu.resp = calloc(apdu.resplen, 1);
	if (apdu.resp == NULL) {
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_NOT_ENOUGH_MEMORY);
	}

	 
	sc_log(card->ctx, "Waiting for the card to generate key...");
	r = sc_transmit_apdu(card, &apdu);
	sc_log(card->ctx, "Card has done key generation.");
	if (r < 0) {
		sc_log(card->ctx, "APDU transmit failed. Error %s.", sc_strerror(r));
		goto finish;
	}

	 
	r = sc_check_sw(card, apdu.sw1, apdu.sw2);
	 
	if (r == SC_ERROR_SECURITY_STATUS_NOT_SATISFIED) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Please verify PIN first.");
		goto finish;
	}

	 
	pgp_parse_and_set_pubkey_output(card, apdu.resp, apdu.resplen, key_info);
	pgp_update_card_algorithms(card, key_info);

finish:
	free(apdu.resp);
	LOG_FUNC_RETURN(card->ctx, r);
}
