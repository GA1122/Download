static int asepcos_delete_file(sc_card_t *card, const sc_path_t *path)
{
	int       r, ftype, atype;
	sc_apdu_t apdu;
	u8        buf[SC_MAX_APDU_BUFFER_SIZE];

	 
	sc_format_apdu(card, &apdu, SC_APDU_CASE_2_SHORT, 0xca, 0x01, 0x84);
	apdu.le      = 256;
	apdu.resplen = sizeof(buf);
	apdu.resp    = buf;
	r = sc_transmit_apdu(card, &apdu);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");
	if (apdu.sw1 == 0x90 && apdu.sw2 == 0x00) {
		 
		atype = SC_APDU_CASE_3_SHORT;
		ftype = 0x02;
		buf[0] = path->value[path->len-2]; 
		buf[1] = path->value[path->len-1];
	} else {
		 
		atype = SC_APDU_CASE_1;
		ftype = 0x00;
	}
	
	sc_format_apdu(card, &apdu, atype, 0xe4, ftype, 0x00);
	if (atype == SC_APDU_CASE_3_SHORT) {
		apdu.lc      = 2;
		apdu.datalen = 2;
		apdu.data    = buf;
	}
	
	r = sc_transmit_apdu(card, &apdu);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");
	return sc_check_sw(card, apdu.sw1, apdu.sw2);
}