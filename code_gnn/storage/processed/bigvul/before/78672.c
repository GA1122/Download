static int rtecp_list_files(sc_card_t *card, u8 *buf, size_t buflen)
{
	sc_apdu_t apdu;
	u8 rbuf[SC_MAX_APDU_BUFFER_SIZE], previd[2];
	const u8 *tag;
	size_t taglen, len = 0;
	int r;

	assert(card && card->ctx && buf);
	sc_format_apdu(card, &apdu, SC_APDU_CASE_2_SHORT, 0xA4, 0, 0);
	for (;;)
	{
		apdu.resp = rbuf;
		apdu.resplen = sizeof(rbuf);
		apdu.le = 256;
		r = sc_transmit_apdu(card, &apdu);
		SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");
		if (apdu.sw1 == 0x6A  &&  apdu.sw2 == 0x82)
			break;  

		r = sc_check_sw(card, apdu.sw1, apdu.sw2);
		SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "");

		if (apdu.resplen <= 2)
			SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_NORMAL, SC_ERROR_WRONG_LENGTH);

		 
		tag = sc_asn1_find_tag(card->ctx, apdu.resp + 2, apdu.resplen - 2,
				0x83, &taglen);
		if (!tag || taglen != sizeof(previd))
			SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_NORMAL, SC_ERROR_UNKNOWN_DATA_RECEIVED);
		memcpy(previd, tag, sizeof(previd));

		if (len + sizeof(previd) <= buflen)
		{
			memcpy(&buf[len], previd, sizeof(previd));
			len += sizeof(previd);
		}

		tag = sc_asn1_find_tag(card->ctx, apdu.resp + 2, apdu.resplen - 2,
				0x82, &taglen);
		if (!tag || taglen != 2)
			SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_NORMAL, SC_ERROR_UNKNOWN_DATA_RECEIVED);
		if (tag[0] == 0x38)
		{
			 
			sc_format_apdu(card, &apdu, SC_APDU_CASE_1, 0xA4, 0x03, 0);
			r = sc_transmit_apdu(card, &apdu);
			SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");
			r = sc_check_sw(card, apdu.sw1, apdu.sw2);
			SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "");
		}
		sc_format_apdu(card, &apdu, SC_APDU_CASE_4_SHORT, 0xA4, 0, 0x02);
		apdu.lc = sizeof(previd);
		apdu.data = previd;
		apdu.datalen = sizeof(previd);
	}
	SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_VERBOSE, len);
}