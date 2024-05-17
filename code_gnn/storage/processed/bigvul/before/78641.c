piv_get_serial_nr_from_CHUI(sc_card_t* card, sc_serial_number_t* serial)
{
	int r;
	int i;
	u8 gbits;
	u8 *rbuf = NULL;
	const u8 *body;
	const u8 *fascn;
	const u8 *guid;
	size_t rbuflen = 0, bodylen, fascnlen, guidlen;

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);
	if (card->serialnr.len)   {
		*serial = card->serialnr;
		LOG_FUNC_RETURN(card->ctx, SC_SUCCESS);
	}

	 

	r = piv_get_cached_data(card, PIV_OBJ_CHUI, &rbuf, &rbuflen);
	LOG_TEST_RET(card->ctx, r, "Failure retrieving CHUI");

	r = SC_ERROR_INTERNAL;
	if (rbuflen != 0) {
		body = sc_asn1_find_tag(card->ctx, rbuf, rbuflen, 0x53, &bodylen);  
		if (body != NULL && bodylen != 0) {
			fascn = sc_asn1_find_tag(card->ctx, body, bodylen, 0x30, &fascnlen);  
			guid = sc_asn1_find_tag(card->ctx, body, bodylen, 0x34, &guidlen);

			gbits = 0;  
			if (guid && guidlen == 16) {
				for (i = 0; i < 16; i++) {
					gbits = gbits | guid[i];  
				}
			}
			sc_log(card->ctx,
			       "fascn=%p,fascnlen=%"SC_FORMAT_LEN_SIZE_T"u,guid=%p,guidlen=%"SC_FORMAT_LEN_SIZE_T"u,gbits=%2.2x",
			       fascn, fascnlen, guid, guidlen, gbits);

			if (fascn && fascnlen == 25) {
				 
				if (!(gbits && fascn[0] == 0xD4 && fascn[1] == 0xE7
						    && fascn[2] == 0x39 && (fascn[3] | 0x7F) == 0xFF)) {
					serial->len = fascnlen < SC_MAX_SERIALNR ? fascnlen : SC_MAX_SERIALNR;
					memcpy (serial->value, fascn, serial->len);
					r = SC_SUCCESS;
					gbits = 0;  
				}
			}
			if (guid && gbits) {
				serial->len = guidlen < SC_MAX_SERIALNR ? guidlen : SC_MAX_SERIALNR;
				memcpy (serial->value, guid, serial->len);
				r = SC_SUCCESS;
			}
		}
	}

	card->serialnr = *serial;
	LOG_FUNC_RETURN(card->ctx, r);
}
