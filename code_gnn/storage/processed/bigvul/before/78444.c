static int gpk_get_serialnr(sc_card_t *card, sc_serial_number_t *serial)
{
	int r;
	u8  rbuf[10];
	sc_apdu_t apdu;

	if (card->type != SC_CARD_TYPE_GPK_GPK16000)
		return SC_ERROR_NOT_SUPPORTED;

	if (!serial)
		return SC_ERROR_INVALID_ARGUMENTS;
	 
	if (card->serialnr.len) {
		memcpy(serial, &card->serialnr, sizeof(*serial));
		return SC_SUCCESS;
	}
	 
	sc_format_apdu(card, &apdu, SC_APDU_CASE_2_SHORT, 0xb8, 0x00, 0x00);
	apdu.cla |= 0x80;
	apdu.resp = rbuf;
	apdu.resplen = sizeof(rbuf);
	apdu.le   = 8;
	apdu.lc   = 0;
	apdu.datalen = 0;
        r = sc_transmit_apdu(card, &apdu);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");
	if (apdu.sw1 != 0x90 || apdu.sw2 != 0x00)
		return SC_ERROR_INTERNAL;
	 
	memcpy(card->serialnr.value, apdu.resp, apdu.resplen);
	card->serialnr.len = apdu.resplen;
	 
	memcpy(serial, &card->serialnr, sizeof(*serial));
	return SC_SUCCESS;
}