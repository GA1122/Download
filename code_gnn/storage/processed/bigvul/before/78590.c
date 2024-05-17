pgp_get_data(sc_card_t *card, unsigned int tag, u8 *buf, size_t buf_len)
{
	sc_apdu_t	apdu;
	int		r;

	LOG_FUNC_CALLED(card->ctx);

	sc_format_apdu(card, &apdu, SC_APDU_CASE_2, 0xCA, tag >> 8, tag);
	apdu.le = ((buf_len >= 256) && !(card->caps & SC_CARD_CAP_APDU_EXT)) ? 256 : buf_len;
	apdu.resp = buf;
	apdu.resplen = buf_len;

	r = sc_transmit_apdu(card, &apdu);
	LOG_TEST_RET(card->ctx, r, "APDU transmit failed");

	r = sc_check_sw(card, apdu.sw1, apdu.sw2);

	 
	if (r == SC_ERROR_DATA_OBJECT_NOT_FOUND && card->type == SC_CARD_TYPE_OPENPGP_GNUK
        && (tag == DO_CERT || tag == DO_PRIV1 || tag == DO_PRIV2 || tag == DO_PRIV3 || tag == DO_PRIV4)) {
		r = SC_SUCCESS;
		apdu.resplen = 0;
	}
	LOG_TEST_RET(card->ctx, r, "Card returned error");

	LOG_FUNC_RETURN(card->ctx, (int)apdu.resplen);
}