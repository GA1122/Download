pgp_parse_and_set_pubkey_output(sc_card_t *card, u8* data, size_t data_len,
                                sc_cardctl_openpgp_keygen_info_t *key_info)
{
	time_t ctime = 0;
	u8 *in = data;
	u8 *modulus = NULL;
	u8 *exponent = NULL;
	int r;
	LOG_FUNC_CALLED(card->ctx);

	 
	r = pgp_store_creationtime(card, key_info->keytype, &ctime);
	LOG_TEST_RET(card->ctx, r, "Cannot store creation time");

	 
	while (data_len > (size_t) (in - data)) {
		unsigned int cla, tag, tmptag;
		size_t		len;
		u8	*part = in;

		 
		r = sc_asn1_read_tag((const u8**)&part,
							 data_len - (in - data),
							 &cla, &tag, &len);
		if (part == NULL)
			r = SC_ERROR_ASN1_OBJECT_NOT_FOUND;
		LOG_TEST_RET(card->ctx, r, "Unexpected end of contents.");
		 
		for (tmptag = tag; tmptag > 0x0FF; tmptag >>= 8) {
			cla <<= 8;
		}
		tag |= cla;

		if (tag == 0x0081) {
			 
			if (key_info->modulus) {
				memcpy(key_info->modulus, part, len);
			}
			 
			key_info->modulus_len = len*8;
			 
			modulus = part;
		}
		else if (tag == 0x0082) {
			 
			if (key_info->exponent) {
				memcpy(key_info->exponent, part, len);
			}
			 
			key_info->exponent_len = len*8;
			 
			exponent = part;
		}

		 
		 
		in = part + ((tag != 0x7F49) ? len : 0);
	}

	 
	sc_log(card->ctx, "Calculate and store fingerprint");
	r = pgp_calculate_and_store_fingerprint(card, ctime, modulus, exponent, key_info);
	LOG_TEST_RET(card->ctx, r, "Cannot store fingerprint.");
	 
	sc_log(card->ctx, "Update blobs holding pubkey info.");
	r = pgp_update_pubkey_blob(card, modulus, key_info->modulus_len,
	                           exponent, key_info->exponent_len, key_info->keytype);
	LOG_FUNC_RETURN(card->ctx, r);
}
