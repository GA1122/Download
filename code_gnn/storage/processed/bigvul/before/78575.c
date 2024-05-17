pgp_build_extended_header_list(sc_card_t *card, sc_cardctl_openpgp_keystore_info_t *key_info,
                               u8 **result, size_t *resultlen)
{
	sc_context_t *ctx = card->ctx;
	 
	const size_t max_prtem_len = 7*(1 + 3);      
	                                             
	u8 pritemplate[7*(1 + 3)];
	size_t tpl_len = 0;      
	 
	u8 kdata[3 + 256 + 256 + 512];   
	size_t kdata_len = 0;    
	u8 *tlvblock = NULL;
	size_t tlvlen = 0;
	u8 *tlv_5f48 = NULL;
	size_t tlvlen_5f48 = 0;
	u8 *tlv_7f48 = NULL;
	size_t tlvlen_7f48 = 0;
	u8 *data = NULL;
	size_t len = 0;
	u8 *p = NULL;
	u8 *components[] = {key_info->e, key_info->p, key_info->q, key_info->n};
	size_t componentlens[] = {key_info->e_len, key_info->p_len, key_info->q_len, key_info->n_len};
	unsigned int componenttags[] = {0x91, 0x92, 0x93, 0x97};
	char *componentnames[] = {
		"public exponent",
		"prime p",
		"prime q",
		"modulus"
	};
	size_t comp_to_add = 3;
	size_t req_e_len = 0;      
	pgp_blob_t *alat_blob;
	u8 i;
	int r;

	LOG_FUNC_CALLED(ctx);

	if (key_info->keyformat == SC_OPENPGP_KEYFORMAT_STDN
		|| key_info->keyformat == SC_OPENPGP_KEYFORMAT_CRTN)
		comp_to_add = 4;

	 
	if (comp_to_add == 4 && (key_info->n == NULL || key_info->n_len == 0)){
		sc_log(ctx, "Error: Modulus required!");
		LOG_FUNC_RETURN(ctx, SC_ERROR_INVALID_ARGUMENTS);
	}

	 
	memset(pritemplate, 0, max_prtem_len);

	 
	alat_blob = pgp_find_blob(card, 0x00C0 | key_info->keytype);
	if (!alat_blob) {
		sc_log(ctx, "Cannot read Algorithm Attributes.");
		LOG_FUNC_RETURN(ctx, SC_ERROR_OBJECT_NOT_FOUND);
	}
	req_e_len = bebytes2ushort(alat_blob->data + 3) >> 3;    
	assert(key_info->e_len <= req_e_len);

	 
	if (key_info->e_len < req_e_len) {
		 
		p = calloc(req_e_len, 1);
		if (!p)
			LOG_FUNC_RETURN(ctx, SC_ERROR_NOT_ENOUGH_MEMORY);
		memcpy(p + req_e_len - key_info->e_len, key_info->e, key_info->e_len);
		key_info->e_len = req_e_len;
		 
		free(key_info->e);
		key_info->e = p;
		components[0] = p;
		componentlens[0] = req_e_len;
	}

	 
	p = pritemplate;

	for (i = 0; i < comp_to_add; i++) {
		sc_log(ctx, "Set Tag+Length for %s (%X).", componentnames[i], componenttags[i]);
		len = set_taglength_tlv(p, componenttags[i], componentlens[i]);
		tpl_len += len;

		 
		memcpy(kdata + kdata_len, components[i], componentlens[i]);
		kdata_len += componentlens[i];

		 
		p += len;
	}

	 

	 
	r = pgp_build_tlv(ctx, 0x7F48, pritemplate, tpl_len, &tlv_7f48, &tlvlen_7f48);
	LOG_TEST_RET(ctx, r, "Failed to build TLV for 7F48.");
	tlv_7f48[0] |= 0x7F;
	r = pgp_build_tlv(ctx, 0x5f48, kdata, kdata_len, &tlv_5f48, &tlvlen_5f48);
	if (r < 0) {
		sc_log(ctx, "Failed to build TLV for 5F48.");
		goto out;
	}

	 
	len = 2 + tlvlen_7f48 + tlvlen_5f48;
	 
	data = calloc(len, 1);
	if (data == NULL) {
		sc_log(ctx, "Not enough memory.");
		r = SC_ERROR_NOT_ENOUGH_MEMORY;
		goto out;
	}
	switch (key_info->keytype) {
	case SC_OPENPGP_KEY_SIGN:
		data[0] = 0xB6;
		break;
	case SC_OPENPGP_KEY_ENCR:
		data[0] = 0xB8;
		break;
	case SC_OPENPGP_KEY_AUTH:
		data[0] = 0xA4;
		break;
	default:
		sc_log(ctx, "Unknown key type %d.", key_info->keytype);
		r = SC_ERROR_INVALID_ARGUMENTS;
		goto out;
	}
	memcpy(data + 2, tlv_7f48, tlvlen_7f48);
	memcpy(data + 2 + tlvlen_7f48, tlv_5f48, tlvlen_5f48);
	r = pgp_build_tlv(ctx, 0x4D, data, len, &tlvblock, &tlvlen);
	if (r < 0) {
		sc_log(ctx, "Cannot build TLV for Extended Header list.");
		goto out;
	}
	 
	if (result != NULL) {
		*result = tlvblock;
		*resultlen = tlvlen;
	} else {
		free(tlvblock);
	}

out:
	free(data);
	free(tlv_5f48);
	free(tlv_7f48);
	LOG_FUNC_RETURN(ctx, r);
}
