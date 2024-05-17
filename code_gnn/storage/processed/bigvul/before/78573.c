gnuk_write_certificate(sc_card_t *card, const u8 *buf, size_t length)
{
	size_t i = 0;
	sc_apdu_t apdu;
	int r = SC_SUCCESS;

	LOG_FUNC_CALLED(card->ctx);

	 
	if (buf == NULL || length == 0) {
		sc_format_apdu(card, &apdu, SC_APDU_CASE_1, 0xD6, 0x85, 0);
		r = sc_transmit_apdu(card, &apdu);
		LOG_TEST_RET(card->ctx, r, "APDU transmit failed");
		 
		LOG_TEST_RET(card->ctx, sc_check_sw(card, apdu.sw1, apdu.sw2), "Certificate writing failed");
	}

	 
	 
	for (i = 0; i*256 < length; i++) {
		u8 *part = (u8 *)buf + i*256;
		size_t plen = MIN(length - i*256, 256);
		u8 roundbuf[256];	 

		sc_log(card->ctx,
		       "Write part %"SC_FORMAT_LEN_SIZE_T"u from offset 0x%"SC_FORMAT_LEN_SIZE_T"X, len %"SC_FORMAT_LEN_SIZE_T"u",
		       i+1, i*256, plen);

		 
		sc_format_apdu(card, &apdu, SC_APDU_CASE_3_SHORT, 0xD6, (i == 0) ? 0x85 : i, 0);
		apdu.flags |= SC_APDU_FLAGS_CHAINING;
		apdu.data = part;
		apdu.datalen = apdu.lc = plen;

		 
		if (plen < 256 && (plen % 2) != 0) {
			memcpy(roundbuf, part, plen);
			roundbuf[plen++] = 0;
			apdu.data = roundbuf;
			apdu.datalen = apdu.lc = plen;
		}

		r = sc_transmit_apdu(card, &apdu);
		LOG_TEST_RET(card->ctx, r, "APDU transmit failed");
		 
		LOG_TEST_RET(card->ctx, sc_check_sw(card, apdu.sw1, apdu.sw2), "UPDATE BINARY returned error");
	}

	LOG_FUNC_RETURN(card->ctx, (int)length);
}