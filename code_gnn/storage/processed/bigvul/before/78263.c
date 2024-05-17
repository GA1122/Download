static int cac_select_file_by_type(sc_card_t *card, const sc_path_t *in_path, sc_file_t **file_out, int type)
{
	struct sc_context *ctx;
	struct sc_apdu apdu;
	unsigned char buf[SC_MAX_APDU_BUFFER_SIZE];
	unsigned char pathbuf[SC_MAX_PATH_SIZE], *path = pathbuf;
	int r, pathlen, pathtype;
	struct sc_file *file = NULL;
	cac_private_data_t * priv = CAC_DATA(card);

	assert(card != NULL && in_path != NULL);
	ctx = card->ctx;

	SC_FUNC_CALLED(ctx, SC_LOG_DEBUG_VERBOSE);

	memcpy(path, in_path->value, in_path->len);
	pathlen = in_path->len;
	pathtype = in_path->type;

	sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE,
		 "path->aid=%x %x %x %x %x %x %x  len=%"SC_FORMAT_LEN_SIZE_T"u, path->value = %x %x %x %x len=%"SC_FORMAT_LEN_SIZE_T"u path->type=%d (%x)",
		 in_path->aid.value[0], in_path->aid.value[1],
		 in_path->aid.value[2], in_path->aid.value[3],
		 in_path->aid.value[4], in_path->aid.value[5],
		 in_path->aid.value[6], in_path->aid.len, in_path->value[0],
		 in_path->value[1], in_path->value[2], in_path->value[3],
		 in_path->len, in_path->type, in_path->type);
	sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "file_out=%p index=%d count=%d\n",
		 file_out, in_path->index, in_path->count);

	 
	if ((pathlen > 2) && (pathlen <= 4) && memcmp(path, "\x3F\x00", 2) == 0) {
		if (pathlen > 2) {
			path += 2;
			pathlen -= 2;
		}
	}


	 
	if (priv) {  
		priv->object_type = CAC_OBJECT_TYPE_GENERIC;
		if (cac_is_cert(priv, in_path)) {
			priv->object_type = CAC_OBJECT_TYPE_CERT;
		}

		 
		if (priv->cache_buf) {
			free(priv->cache_buf);
			priv->cache_buf = NULL;
		}
		priv->cache_buf_len = 0;
		priv->cached = 0;
	}

	if (in_path->aid.len) {
		if (!pathlen) {
			memcpy(path, in_path->aid.value, in_path->aid.len);
			pathlen = in_path->aid.len;
			pathtype = SC_PATH_TYPE_DF_NAME;
		} else {
			 
			sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE,"select application" );
			sc_format_apdu(card, &apdu, SC_APDU_CASE_3_SHORT, 0xA4, 4, 0);
			apdu.data = in_path->aid.value;
			apdu.datalen = in_path->aid.len;
			apdu.lc = in_path->aid.len;

			r = sc_transmit_apdu(card, &apdu);
			LOG_TEST_RET(ctx, r, "APDU transmit failed");
			r = sc_check_sw(card, apdu.sw1, apdu.sw2);
			if (r)
				LOG_FUNC_RETURN(ctx, r);

		}
	}

	sc_format_apdu(card, &apdu, SC_APDU_CASE_4_SHORT, 0xA4, 0, 0);

	switch (pathtype) {
	 
	case SC_PATH_TYPE_FILE_ID:
		apdu.p1 = 2;
		if (pathlen != 2)
			return SC_ERROR_INVALID_ARGUMENTS;
		break;
	case SC_PATH_TYPE_DF_NAME:
		apdu.p1 = 4;
		break;
	default:
		LOG_FUNC_RETURN(ctx, SC_ERROR_INVALID_ARGUMENTS);
	}
	apdu.lc = pathlen;
	apdu.data = path;
	apdu.datalen = pathlen;
	apdu.resp = buf;
	apdu.resplen = sizeof(buf);
	apdu.le = sc_get_max_recv_size(card) < 256 ? sc_get_max_recv_size(card) : 256;

	if (file_out != NULL) {
		apdu.p2 = 0;		 
	}
	else {
		apdu.p2 = 0x0C;
	}

	r = sc_transmit_apdu(card, &apdu);
	LOG_TEST_RET(ctx, r, "APDU transmit failed");

	if (file_out == NULL) {
		 
		r = sc_check_sw(card, apdu.sw1, apdu.sw2);
		if (apdu.sw1 == 0x6A && apdu.sw2 == 0x86)   {
			apdu.p2 = 0x00;
			apdu.resplen = sizeof(buf);
			if (sc_transmit_apdu(card, &apdu) == SC_SUCCESS)
				r = sc_check_sw(card, apdu.sw1, apdu.sw2);
		}
		if (apdu.sw1 == 0x61)
			LOG_FUNC_RETURN(ctx, SC_SUCCESS);
		LOG_FUNC_RETURN(ctx, r);
	}

	r = sc_check_sw(card, apdu.sw1, apdu.sw2);
	if (r)
		LOG_FUNC_RETURN(ctx, r);

	 
	if (priv && in_path->len >= 2) {
		 
		cac_properties_t prop;
		size_t i = -1;

		r = cac_get_properties(card, &prop);
		if (r == SC_SUCCESS) {
			for (i = 0; i < prop.num_objects; i++) {
				sc_log(card->ctx, "Searching for our OID: 0x%02x 0x%02x = 0x%02x 0x%02x",
				    prop.objects[i].oid[0], prop.objects[i].oid[1],
					in_path->value[0], in_path->value[1]);
				if (memcmp(prop.objects[i].oid,
				    in_path->value, 2) == 0)
					break;
			}
		}
		if (i < prop.num_objects) {
			if (prop.objects[i].privatekey)
				priv->object_type = CAC_OBJECT_TYPE_CERT;
			else if (prop.objects[i].simpletlv == 0)
				priv->object_type = CAC_OBJECT_TYPE_TLV_FILE;
		}
	}

	 
	file = sc_file_new();
	if (file == NULL)
			LOG_FUNC_RETURN(ctx, SC_ERROR_OUT_OF_MEMORY);
	file->path = *in_path;
	file->size = CAC_MAX_SIZE;  

	*file_out = file;
	SC_FUNC_RETURN(ctx, SC_LOG_DEBUG_NORMAL, SC_SUCCESS);

}
