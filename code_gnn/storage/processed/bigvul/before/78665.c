static int rtecp_change_reference_data(sc_card_t *card, unsigned int type,
		int ref_qualifier, const u8 *old, size_t oldlen,
		const u8 *newref, size_t newlen, int *tries_left)
{
	sc_apdu_t apdu;
	u8 rsf_length[2], *buf, *buf_end, *p; 
	size_t val_length, buf_length, max_transmit_length;
	int transmits_num, r;

	assert(card && card->ctx && newref);
	sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL,
		 "newlen = %"SC_FORMAT_LEN_SIZE_T"u\n", newlen);
	if (newlen > 0xFFFF)
		SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_NORMAL, SC_ERROR_INVALID_ARGUMENTS);
	if (type == SC_AC_CHV && old && oldlen != 0)
	{
		r = sc_verify(card, type, ref_qualifier, old, oldlen, tries_left);
		SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "Verify old pin failed");
	}
	
	max_transmit_length = sc_get_max_send_size(card);
	assert(max_transmit_length > 2);
	 
	transmits_num = (2 + sizeof(rsf_length) + newlen) / (max_transmit_length - 2) + 1;
	 
	buf_length = (2 + sizeof(rsf_length)) + newlen + 2*(transmits_num); 
	p = buf = (u8 *)malloc(buf_length);
	if (buf == NULL)
		SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_NORMAL, SC_ERROR_OUT_OF_MEMORY);
	buf_end = buf + buf_length; 

	sc_format_apdu(card, &apdu, SC_APDU_CASE_3_SHORT, 0x24, 0x01, ref_qualifier);	
	 
	rsf_length[0] = (newlen >> 8) & 0xFF;
	rsf_length[1] = newlen & 0xFF;
	assert(buf_end - p >= (int)(2 + sizeof(rsf_length)));
	sc_asn1_put_tag(0x80, rsf_length, sizeof(rsf_length), p, buf_end - p, &p);
	 
	while (newlen)
	{
		assert(buf_end - p >= (int)(newlen + 2));
		if ((p - buf) % max_transmit_length + newlen + 2 > max_transmit_length)
			val_length = max_transmit_length - (p - buf) % max_transmit_length - 2;
		else
			val_length = newlen;
		 
		*p++ = 0xA5;
		*p++ = (u8)val_length;
		assert(val_length <= newlen);
		memcpy(p, newref, val_length);
		p += val_length;
		newref += val_length;
		newlen -= val_length;
		if (newlen)
			apdu.flags |= SC_APDU_FLAGS_CHAINING;
	}
	apdu.lc = p - buf;
	apdu.data = buf;
	apdu.datalen = p - buf;

	r = sc_transmit_apdu(card, &apdu);
	sc_mem_clear(buf, buf_length);
	free(buf);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");
	r = sc_check_sw(card, apdu.sw1, apdu.sw2);
	SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_VERBOSE, r);
}
