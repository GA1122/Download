static int rtecp_reset_retry_counter(sc_card_t *card, unsigned int type,
		int ref_qualifier, const u8 *puk, size_t puklen,
		const u8 *newref, size_t newlen)
{
	sc_apdu_t apdu;
	int r;

	(void)type, (void)puk, (void)puklen;  
	assert(card && card->ctx);

	sc_format_apdu(card, &apdu, SC_APDU_CASE_1, 0x2C, 0x03, ref_qualifier);
	r = sc_transmit_apdu(card, &apdu);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");
	r = sc_check_sw(card, apdu.sw1, apdu.sw2);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "Unblock card failed");
	
	if (newref && newlen)   {
        	u8 tmp[2], buf[SC_MAX_APDU_BUFFER_SIZE];
		u8 *p = buf;

		tmp[0] = (newlen >> 8) & 0xFF;
		tmp[1] = newlen & 0xFF;
		sc_asn1_put_tag(0x80, tmp, sizeof(tmp), p, sizeof(buf) - (p - buf), &p);
		r = sc_asn1_put_tag(0xA5, newref, newlen, p, sizeof(buf) - (p - buf), &p);
		SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "Invalid new PIN length");

		sc_format_apdu(card, &apdu, SC_APDU_CASE_3_SHORT, 0x24, 0x01, ref_qualifier);
		apdu.lc = p - buf;
		apdu.data = buf;
		apdu.datalen = p - buf;

		r = sc_transmit_apdu(card, &apdu);
		SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");
		r = sc_check_sw(card, apdu.sw1, apdu.sw2);
		SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "Set PIN failed");
	}

	SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_VERBOSE, r);
}