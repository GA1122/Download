static int asepcos_create_file(sc_card_t *card, sc_file_t *file)
{
	if (file->type == SC_FILE_TYPE_DF) {
		int r, type;
		sc_apdu_t apdu;
		u8  sbuf[SC_MAX_APDU_BUFFER_SIZE], *p = &sbuf[0];

		*p++ = (file->id >> 8) & 0xff;
		*p++ = file->id & 0xff;
		if (file->size > 0xffff) {
			*p++ = (file->size >> 24) & 0xff;
			*p++ = (file->size >> 16) & 0xff;
			*p++ = (file->size >> 8 ) & 0xff;
			*p++ = file->size & 0xff;
			type = 1;
		} else {
			*p++ = (file->size >> 8) & 0xff;
			*p++ = file->size & 0xff;
			type = 0;
		}
		if (file->namelen != 0 && file->namelen <= 16) {
			memcpy(p, file->name, file->namelen);
			p += file->namelen;
		}
		sc_format_apdu(card, &apdu, SC_APDU_CASE_3_SHORT, 0xe0, 0x38, type);
		apdu.cla    |= 0x80;
		apdu.lc      = p - sbuf;
		apdu.datalen = p - sbuf;
		apdu.data    = sbuf;

		r = sc_transmit_apdu(card, &apdu);
		SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");
		if (apdu.sw1 != 0x90 || apdu.sw2 != 0x00)
			return sc_check_sw(card, apdu.sw1, apdu.sw2); 

		r = sc_select_file(card, &file->path, NULL);
		if (r != SC_SUCCESS)
			return r;
		 
		r = asepcos_set_security_attributes(card, file);
		if (r != SC_SUCCESS) {
			sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "unable to set security attributes");
			return r;
		}
		return SC_SUCCESS;
	} else if (file->type == SC_FILE_TYPE_WORKING_EF) {
		int r;
		sc_apdu_t apdu;
		u8  descr_byte = file->ef_structure & 7;
		u8  sbuf[SC_MAX_APDU_BUFFER_SIZE], *p = &sbuf[0];

		*p++ = 0x85;
		p++;
		 
		*p++ = (file->id >> 8) & 0xff;
		*p++ = file->id & 0xff;
		 
		if (file->ef_structure == SC_FILE_EF_TRANSPARENT) {
			*p++ = 0x00;
			*p++ = 0x00;
		} else {
			*p++ = (file->record_length >> 8) & 0xff;
			*p++ = file->record_length & 0xff;
		}
		 
		if (file->ef_structure == SC_FILE_EF_TRANSPARENT) {
			*p++ = (file->size >> 8) & 0xff;
			*p++ = file->size & 0xff;
		} else {
			*p++ = (file->record_count >> 8) & 0xff;
			*p++ = file->record_count & 0xff;
		}
		 
		sbuf[1] = p - sbuf - 2;		 

		sc_format_apdu(card, &apdu, SC_APDU_CASE_3_SHORT, 0xe0, descr_byte, 0x00);
		apdu.lc      = p - sbuf;
		apdu.datalen = p - sbuf;
		apdu.data    = sbuf;
		r = sc_transmit_apdu(card, &apdu);
		SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");
		if (apdu.sw1 != 0x90 || apdu.sw2 != 0x00)
			return sc_check_sw(card, apdu.sw1, apdu.sw2);

		 
		r = asepcos_set_security_attributes(card, file);
		if (r != SC_SUCCESS) {
			sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "unable to set security attributes");
			return r;
		}
		return asepcos_activate_file(card, file->id, 1);
	} else if (file->type == SC_FILE_TYPE_INTERNAL_EF) {
		 
		int r, atype = SC_APDU_CASE_3_SHORT;
		sc_apdu_t apdu;

		if (file->prop_attr_len > 255)
			atype = SC_APDU_CASE_3_EXT;

		sc_format_apdu(card, &apdu, atype, 0xe0, 0x08, 0x00);
		apdu.lc      = file->prop_attr_len;
		apdu.datalen = file->prop_attr_len;
		apdu.data    = file->prop_attr;

		r = sc_transmit_apdu(card, &apdu);
		SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");
		if (apdu.sw1 != 0x90 || apdu.sw2 != 0x00)
			return sc_check_sw(card, apdu.sw1, apdu.sw2);
		 
		r = asepcos_set_security_attributes(card, file);
		if (r != SC_SUCCESS) {
			sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "unable to set security attributes");
			return r;
		}
		return asepcos_activate_file(card, file->id, 1);
	} else
		return SC_ERROR_INVALID_ARGUMENTS;
}