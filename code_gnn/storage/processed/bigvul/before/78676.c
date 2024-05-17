static int rtecp_verify(sc_card_t *card, unsigned int type, int ref_qualifier,
		const u8 *data, size_t data_len, int *tries_left)
{
	sc_apdu_t apdu;
	int r, send_logout = 0;

	(void)type;  
	assert(card && card->ctx && data);
	for (;;)
	{
		sc_format_apdu(card, &apdu, SC_APDU_CASE_3_SHORT,
				0x20, 0, ref_qualifier);
		apdu.lc = data_len;
		apdu.data = data;
		apdu.datalen = data_len;
		r = sc_transmit_apdu(card, &apdu);
		SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");
		if (send_logout++ == 0 && apdu.sw1 == 0x6F && apdu.sw2 == 0x86)
		{
			 r = sc_logout(card);
			 SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "Logout failed");
		}
		else
			break;
	}
	if (apdu.sw1 == 0x63 && apdu.sw2 == 0)
	{
		 
		sc_format_apdu(card, &apdu, SC_APDU_CASE_1, 0x20, 0, ref_qualifier);
		r = sc_transmit_apdu(card, &apdu);
		SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");
	}
	r = sc_check_sw(card, apdu.sw1, apdu.sw2);
	if (r == SC_ERROR_PIN_CODE_INCORRECT && tries_left)
		*tries_left = (int)(apdu.sw2 & 0x0F);
	SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_VERBOSE, r);
}