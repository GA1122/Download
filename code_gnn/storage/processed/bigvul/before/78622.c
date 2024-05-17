piv_cache_internal_data(sc_card_t *card, int enumtag)
{
	piv_private_data_t * priv = PIV_DATA(card);
	const u8* tag;
	const u8* body;
	size_t taglen;
	size_t bodylen;
	int compressed = 0;

	 
	if (priv->obj_cache[enumtag].internal_obj_data && priv->obj_cache[enumtag].internal_obj_len) {
		sc_log(card->ctx,
		       "#%d found internal %p:%"SC_FORMAT_LEN_SIZE_T"u",
		       enumtag,
		       priv->obj_cache[enumtag].internal_obj_data,
		       priv->obj_cache[enumtag].internal_obj_len);
		LOG_FUNC_RETURN(card->ctx, SC_SUCCESS);
	}

	body = sc_asn1_find_tag(card->ctx,
			priv->obj_cache[enumtag].obj_data,
			priv->obj_cache[enumtag].obj_len,
			0x53, &bodylen);

	if (body == NULL)
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_OBJECT_NOT_VALID);

	 
	 if (piv_objects[enumtag].flags & PIV_OBJECT_TYPE_CERT) {

		tag = sc_asn1_find_tag(card->ctx, body, bodylen, 0x71, &taglen);
		 
		 
		if (tag && (((*tag) & 0x80) || ((*tag) & 0x01)))
			compressed = 1;

		tag = sc_asn1_find_tag(card->ctx, body, bodylen, 0x70, &taglen);
		if (tag == NULL)
			LOG_FUNC_RETURN(card->ctx, SC_ERROR_OBJECT_NOT_VALID);

		if (taglen == 0)
			LOG_FUNC_RETURN(card->ctx, SC_ERROR_FILE_NOT_FOUND);

		if(compressed) {
#ifdef ENABLE_ZLIB
			size_t len;
			u8* newBuf = NULL;

			if(SC_SUCCESS != sc_decompress_alloc(&newBuf, &len, tag, taglen, COMPRESSION_AUTO))
				LOG_FUNC_RETURN(card->ctx, SC_ERROR_OBJECT_NOT_VALID);

			priv->obj_cache[enumtag].internal_obj_data = newBuf;
			priv->obj_cache[enumtag].internal_obj_len = len;
#else
			sc_log(card->ctx, "PIV compression not supported, no zlib");
			LOG_FUNC_RETURN(card->ctx, SC_ERROR_NOT_SUPPORTED);
#endif
		}
		else {
			if (!(priv->obj_cache[enumtag].internal_obj_data = malloc(taglen)))
				LOG_FUNC_RETURN(card->ctx, SC_ERROR_OUT_OF_MEMORY);

			memcpy(priv->obj_cache[enumtag].internal_obj_data, tag, taglen);
			priv->obj_cache[enumtag].internal_obj_len = taglen;
		}

	 
 
	}
	else if (piv_objects[enumtag].flags & PIV_OBJECT_TYPE_PUBKEY) {
		tag = sc_asn1_find_tag(card->ctx, body, bodylen, *body, &taglen);
		if (tag == NULL)
			LOG_FUNC_RETURN(card->ctx, SC_ERROR_OBJECT_NOT_VALID);

		if (taglen == 0)
			LOG_FUNC_RETURN(card->ctx, SC_ERROR_FILE_NOT_FOUND);

		if (!(priv->obj_cache[enumtag].internal_obj_data = malloc(taglen)))
			LOG_FUNC_RETURN(card->ctx, SC_ERROR_OUT_OF_MEMORY);

		memcpy(priv->obj_cache[enumtag].internal_obj_data, tag, taglen);
		priv->obj_cache[enumtag].internal_obj_len = taglen;
	}
	else {
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_INTERNAL);
	}

	sc_log(card->ctx, "added #%d internal %p:%"SC_FORMAT_LEN_SIZE_T"u",
	       enumtag,
	       priv->obj_cache[enumtag].internal_obj_data,
	       priv->obj_cache[enumtag].internal_obj_len);

	LOG_FUNC_RETURN(card->ctx, SC_SUCCESS);
}