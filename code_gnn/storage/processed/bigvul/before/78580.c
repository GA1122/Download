pgp_decipher(sc_card_t *card, const u8 *in, size_t inlen,
		u8 *out, size_t outlen)
{
	struct pgp_priv_data	*priv = DRVDATA(card);
	sc_security_env_t	*env = &priv->sec_env;
	sc_apdu_t	apdu;
	u8 apdu_case = SC_APDU_CASE_4;
	u8		*temp = NULL;
	int		r;

	LOG_FUNC_CALLED(card->ctx);

	 
	if (!(temp = malloc(inlen + 1)))
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_OUT_OF_MEMORY);
	 
	temp[0] = 0x00;
	memcpy(temp + 1, in, inlen);
	in = temp;
	inlen += 1;

	if (env->operation != SC_SEC_OPERATION_DECIPHER) {
		free(temp);
		LOG_TEST_RET(card->ctx, SC_ERROR_INVALID_ARGUMENTS,
				"invalid operation");
	}

	switch (env->key_ref[0]) {
	case 0x01:  
	case 0x02:  
		 
		sc_format_apdu(card, &apdu, apdu_case, 0x2A, 0x80, 0x86);
		break;
	case 0x00:  
	default:
		free(temp);
		LOG_TEST_RET(card->ctx, SC_ERROR_INVALID_ARGUMENTS,
				"invalid key reference");
	}

	 
	if (card->type == SC_CARD_TYPE_OPENPGP_GNUK) {
		apdu.flags |= SC_APDU_FLAGS_CHAINING;
	}
	 
	if (((card->caps & SC_CARD_CAP_APDU_EXT) == 0) && (priv->ext_caps & EXT_CAP_CHAINING))
		apdu.flags |= SC_APDU_FLAGS_CHAINING;

	apdu.lc = inlen;
	apdu.data = (u8 *)in;
	apdu.datalen = inlen;
	apdu.le = ((outlen >= 256) && !(card->caps & SC_CARD_CAP_APDU_EXT)) ? 256 : outlen;
	apdu.resp = out;
	apdu.resplen = outlen;

	r = sc_transmit_apdu(card, &apdu);
	free(temp);
	LOG_TEST_RET(card->ctx, r, "APDU transmit failed");

	r = sc_check_sw(card, apdu.sw1, apdu.sw2);
	LOG_TEST_RET(card->ctx, r, "Card returned error");

	LOG_FUNC_RETURN(card->ctx, (int)apdu.resplen);
}