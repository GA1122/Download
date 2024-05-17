static int setcos_match_card(sc_card_t *card)
{
	sc_apdu_t apdu;
	u8 buf[6];
	int i;

	i = _sc_match_atr(card, setcos_atrs, &card->type);
	if (i < 0) {
		 
		if (match_hist_bytes(card, "FinEID", 0)) {
			card->type = SC_CARD_TYPE_SETCOS_FINEID_V2_2048;
			return 1;
		}
		if (match_hist_bytes(card, "FISE", 0)) {
			card->type = SC_CARD_TYPE_SETCOS_GENERIC;
			return 1;
		}
		 
		sc_format_apdu(card, &apdu, SC_APDU_CASE_2_SHORT, 0xCA, 0xDF, 0x30);
		apdu.cla = 0x00;
		apdu.resp = buf;
		apdu.resplen = 5;
		apdu.le = 5;
		i = sc_transmit_apdu(card, &apdu);
		if (i == 0 && apdu.sw1 == 0x90 && apdu.sw2 == 0x00 && apdu.resplen == 5) {
			if (memcmp(buf, "v2.0", 4) == 0)
				card->type = SC_CARD_TYPE_SETCOS_EID_V2_0;
			else if (memcmp(buf, "v2.1", 4) == 0)
				card->type = SC_CARD_TYPE_SETCOS_EID_V2_1;
			else {
				buf[sizeof(buf) - 1] = '\0';
				sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "SetCOS EID applet %s is not supported", (char *) buf);
				return 0;
			}
			return 1;
		}

		return 0;
	}
	card->flags = setcos_atrs[i].flags;
	return 1;
}