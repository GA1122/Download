gpk_compute_signature(sc_card_t *card, const u8 *data,
		size_t data_len, u8 * out, size_t outlen)
{
	struct gpk_private_data *priv = DRVDATA(card);
	sc_apdu_t	apdu;
	u8		cardsig[1024/8];
	int		r;

	if (data_len > priv->sec_mod_len) {
		sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL,
			 "Data length (%"SC_FORMAT_LEN_SIZE_T"u) does not match key modulus %u.\n",
			 data_len, priv->sec_mod_len);
		return SC_ERROR_INTERNAL;
	}
	if (sizeof(cardsig) < priv->sec_mod_len)
		return SC_ERROR_BUFFER_TOO_SMALL;

	r = gpk_init_hashed(card, data, data_len);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "Failed to send hash to card");

	 
	memset(&apdu, 0, sizeof(apdu));
	apdu.cse = SC_APDU_CASE_2_SHORT;
	apdu.cla = 0x80;
	apdu.ins = 0x86;
	apdu.p2  = priv->sec_padding;
	apdu.resp= cardsig;
	apdu.resplen = sizeof(cardsig);
	apdu.le  = priv->sec_mod_len;

	r = sc_transmit_apdu(card, &apdu);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");
	r = sc_check_sw(card, apdu.sw1, apdu.sw2);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "Card returned error");

	 
	r = reverse(out, outlen, cardsig, apdu.resplen);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "Failed to reverse signature");

	return r;
}