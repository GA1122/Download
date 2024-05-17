gpk_lock(sc_card_t *card, struct sc_cardctl_gpk_lock *args)
{
	struct gpk_private_data *priv = DRVDATA(card);
	sc_file_t	*file = args->file;
	sc_apdu_t	apdu;
	u8		data[8], crycks[3], resp[3];
	int		r;

	sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL,
		"gpk_lock(0x%04X, %u)\n", file->id, args->operation);

	memset(data, 0, sizeof(data));
	data[0] = file->id >> 8;
	data[1] = file->id;
	switch (args->operation) {
	case SC_AC_OP_UPDATE:
		data[2] = 0x40; break;
	case SC_AC_OP_WRITE:
		data[3] = 0x40; break;
	case SC_AC_OP_READ:
		data[4] = 0x40; break;
	default:
		return SC_ERROR_INVALID_ARGUMENTS;
	}

	memset(&apdu, 0, sizeof(apdu));
	apdu.cse = SC_APDU_CASE_3_SHORT;
	apdu.cla = 0x80;
	apdu.ins = 0x16;
	apdu.p1  = (file->type == SC_FILE_TYPE_DF)? 1 : 2;
	apdu.p2  = 0;
	apdu.lc  = 5;
	apdu.datalen = 5;
	apdu.data = data;

	if (priv->key_set) {
		apdu.cla = 0x84;
		apdu.cse = SC_APDU_CASE_4_SHORT;
		r = gpk_compute_crycks(card, &apdu, crycks);
		if (r)
			return r;
		apdu.resp = resp;
		apdu.resplen = sizeof(resp);  
	}

	r = sc_transmit_apdu(card, &apdu);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");
	r = sc_check_sw(card, apdu.sw1, apdu.sw2);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "Card returned error");

	if (priv->key_set)
		r = gpk_verify_crycks(card, &apdu, crycks);

	return r;
}