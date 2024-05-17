static int asepcos_decipher(sc_card_t *card, const u8 * crgram, size_t crgram_len,
	u8 * out, size_t outlen)
{
	int       r;
	sc_apdu_t apdu;

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_NORMAL);

	 
	sc_format_apdu(card, &apdu, SC_APDU_CASE_4, 0x14, 0x01, 0x00);
	apdu.cla    |= 0x80;
	apdu.resp    = out;
	apdu.resplen = outlen;
	 
	apdu.le      = (outlen >= 256 && crgram_len < 256) ? 256 : outlen;
	
	apdu.data    = crgram;
	apdu.lc      = crgram_len;
	apdu.datalen = crgram_len;
	r = sc_transmit_apdu(card, &apdu);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");
	if (apdu.sw1 != 0x90 || apdu.sw2 != 0x00)
		SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_VERBOSE, sc_check_sw(card, apdu.sw1, apdu.sw2));
	return apdu.resplen;
}