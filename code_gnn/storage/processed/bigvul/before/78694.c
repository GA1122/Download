static int setcos_construct_fci_44(sc_card_t *card, const sc_file_t *file, u8 *out, size_t *outlen)
{
	u8 *p = out;
	u8 buf[64];
	const u8 *pin_key_info;
	int len;

	 
	*p++ = 0x6F;
	p++;

	 
	if (SETCOS_IS_EID_APPLET(card) &&
	    (file->type == SC_FILE_TYPE_INTERNAL_EF || 
	     (file->type == SC_FILE_TYPE_WORKING_EF && file->ef_structure == 0x22)))
	     	buf[0] = buf[1] = 0x00;
	else {
		buf[0] = (file->size >> 8) & 0xFF;
		buf[1] = file->size & 0xFF;
	}
	sc_asn1_put_tag(0x81, buf, 2, p, *outlen - (p - out), &p);

	 
	if (file->type_attr_len) {
		memcpy(buf, file->type_attr, file->type_attr_len);
		sc_asn1_put_tag(0x82, buf, file->type_attr_len, p, *outlen - (p - out), &p);
	} else {
		u8	bLen = 1;

		buf[0] = file->shareable ? 0x40 : 0;
		switch (file->type) {
		case SC_FILE_TYPE_INTERNAL_EF:				 
			buf[0] = 0x11;				
			break;
		case SC_FILE_TYPE_WORKING_EF:
			if (file->ef_structure == 0x22) {		 
				buf[0] = 0x0A;				 
				if (SETCOS_IS_EID_APPLET(card))
					bLen = 1;
				else {
					 
					bLen = 5;
					buf[1] = 0x41;				 
					buf[2] = file->record_length >> 8;	 
					buf[3] = file->record_length & 0xFF;
					buf[4] = file->size / file->record_length;  
				}
			} else {
				buf[0] |= file->ef_structure & 7;	 
			}
			break;
		case SC_FILE_TYPE_DF:	
			buf[0] = 0x38;
			break;
		default:
			return SC_ERROR_NOT_SUPPORTED;
		}
		sc_asn1_put_tag(0x82, buf, bLen, p, *outlen - (p - out), &p);
	}

	 
	buf[0] = (file->id >> 8) & 0xFF;
	buf[1] = file->id & 0xFF;
	sc_asn1_put_tag(0x83, buf, 2, p, *outlen - (p - out), &p);

	 
	if (file->type == SC_FILE_TYPE_DF) {
		if (file->name[0] != 0)
			sc_asn1_put_tag(0x84, (u8 *) file->name, file->namelen, p, *outlen - (p - out), &p);
		else {  
			buf[0] = (file->id >> 8) & 0xFF;
			buf[1] = file->id & 0xFF;
			sc_asn1_put_tag(0x84, buf, 2, p, *outlen - (p - out), &p);
		}
	}

	 
	memcpy(buf, file->sec_attr, file->sec_attr_len);
	sc_asn1_put_tag(0x86, buf, file->sec_attr_len, p, *outlen - (p - out), &p);

	 
	if (file->prop_attr_len) {
		memcpy(buf, file->prop_attr, file->prop_attr_len);
		sc_asn1_put_tag(0x8A, buf, file->prop_attr_len, p, *outlen - (p - out), &p);
	}

	 
	if (file->type == SC_FILE_TYPE_DF) {
		if (card->type == SC_CARD_TYPE_SETCOS_EID_V2_1) {
			pin_key_info = (const u8*)"\xC1\x04\x81\x82\x83\x84";
			len = 6;
		}
		else if (card->type == SC_CARD_TYPE_SETCOS_EID_V2_0) {
			pin_key_info = (const u8*)"\xC1\x04\x81\x82";  
			len = 4;
		}
		else {
			 
			if(file->path.len == 2)
				pin_key_info = (const u8*)"\xC1\x04\x81\x82\x83\x84\xC2\x00";	 
			else
				pin_key_info = (const u8 *)"\xC1\x04\x01\x02\x03\x04\xC2\x00";	 
			len = 8;
		}
		sc_asn1_put_tag(0xA5, pin_key_info, len, p, *outlen - (p - out), &p);
	}

	 
	out[1] = p - out - 2;

	*outlen = p - out;
	return 0;
}
