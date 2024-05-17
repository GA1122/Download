static int gpk_get_info(sc_card_t *card, int p1, int p2, u8 *buf,
		size_t buflen)
{
	sc_apdu_t	apdu;
	int	r, retry = 0;

	 
	r = sc_lock(card);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "sc_lock() failed");

	do {
		memset(&apdu, 0, sizeof(apdu));
		apdu.cse = SC_APDU_CASE_2_SHORT;
		apdu.cla = 0x80;
		apdu.ins = 0xC0;
		apdu.p1  = p1;
		apdu.p2  = p2;
		apdu.le  = buflen;
		apdu.resp = buf;
		apdu.resplen = buflen;

		if ((r = sc_transmit_apdu(card, &apdu)) < 0) {
			sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "APDU transmit failed: %s",
					sc_strerror(r));
			sc_unlock(card);
			return r;
		}
	} while (apdu.sw1 == 0x6B && apdu.sw2 == 0x00 && retry++ < 1);
	sc_unlock(card);

	r = sc_check_sw(card, apdu.sw1, apdu.sw2);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "Card returned error");

	return r;
}