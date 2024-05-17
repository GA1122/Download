pgp_compute_signature(sc_card_t *card, const u8 *data,
                size_t data_len, u8 * out, size_t outlen)
{
	struct pgp_priv_data	*priv = DRVDATA(card);
	sc_security_env_t	*env = &priv->sec_env;
	sc_apdu_t		apdu;
	u8 apdu_case = (card->type == SC_CARD_TYPE_OPENPGP_GNUK)
			? SC_APDU_CASE_4_SHORT : SC_APDU_CASE_4;
	int			r;

	LOG_FUNC_CALLED(card->ctx);

	if (env->operation != SC_SEC_OPERATION_SIGN)
		LOG_TEST_RET(card->ctx, SC_ERROR_INVALID_ARGUMENTS,
				"invalid operation");

	switch (env->key_ref[0]) {
	case 0x00:  
		 
		sc_format_apdu(card, &apdu, apdu_case, 0x2A, 0x9E, 0x9A);
		break;
	case 0x02:  
		 
		sc_format_apdu(card, &apdu, apdu_case, 0x88, 0, 0);
		break;
	case 0x01:
	default:
		LOG_TEST_RET(card->ctx, SC_ERROR_INVALID_ARGUMENTS,
			"invalid key reference");
	}

	 
	if (((card->caps & SC_CARD_CAP_APDU_EXT) == 0) && (priv->ext_caps & EXT_CAP_CHAINING))
		apdu.flags |= SC_APDU_FLAGS_CHAINING;

	apdu.lc = data_len;
	apdu.data = (u8 *)data;
	apdu.datalen = data_len;
	apdu.le = ((outlen >= 256) && !(card->caps & SC_CARD_CAP_APDU_EXT)) ? 256 : outlen;
	apdu.resp    = out;
	apdu.resplen = outlen;

	r = sc_transmit_apdu(card, &apdu);
	LOG_TEST_RET(card->ctx, r, "APDU transmit failed");

	r = sc_check_sw(card, apdu.sw1, apdu.sw2);
	LOG_TEST_RET(card->ctx, r, "Card returned error");

	LOG_FUNC_RETURN(card->ctx, (int)apdu.resplen);
}