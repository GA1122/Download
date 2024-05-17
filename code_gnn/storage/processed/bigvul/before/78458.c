gpk_select_key(sc_card_t *card, int key_sfi, const u8 *buf, size_t buflen)
{
	struct gpk_private_data *priv = DRVDATA(card);
	sc_apdu_t	apdu;
	u8		rnd[8], resp[258];
	int		r;

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);

	if (buflen != 16)
		return SC_ERROR_INVALID_ARGUMENTS;

	 
	RAND_bytes(rnd, sizeof(rnd));
	memset(&apdu, 0, sizeof(apdu));
	apdu.cla = 0x80;
	apdu.cse = SC_APDU_CASE_4_SHORT;
	apdu.ins = 0x28;
	apdu.p1  = 0;
	apdu.p2  = key_sfi;
	apdu.data = rnd;
	apdu.datalen = sizeof(rnd);
	apdu.lc = apdu.datalen;
	apdu.resp = resp;
	apdu.resplen = sizeof(resp);
	apdu.le = 12;
	
	r = sc_transmit_apdu(card, &apdu);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");
	r = sc_check_sw(card, apdu.sw1, apdu.sw2);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "Card returned error");

	if (apdu.resplen != 12) {
		r = SC_ERROR_UNKNOWN_DATA_RECEIVED;
	} else
	if ((r = gpk_set_filekey(buf, rnd, resp, priv->key)) == 0) {
		priv->key_set = 1;
		priv->key_reference = key_sfi;
	}

	sc_mem_clear(resp, sizeof(resp));
	return r;
}