piv_read_binary(sc_card_t *card, unsigned int idx, unsigned char *buf, size_t count, unsigned long flags)
{
	piv_private_data_t * priv = PIV_DATA(card);
	int enumtag;
	int r;
	u8 *rbuf = NULL;
	size_t rbuflen = 0;
	const u8 *body;
	size_t bodylen;

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);
	if (priv->selected_obj < 0)
		 LOG_FUNC_RETURN(card->ctx, SC_ERROR_INTERNAL);
	enumtag = piv_objects[priv->selected_obj].enumtag;

	if (priv->rwb_state == -1) {
		r = piv_get_cached_data(card, enumtag, &rbuf, &rbuflen);

		if (r >=0) {
			 
			 
			if (!rbuf || rbuf[0] == 0x00 || ((rbuf[0]&0xDF) == 0x53 && rbuf[1] == 0x00)) {
				r = SC_ERROR_FILE_NOT_FOUND;
				goto err;
			}
			sc_log(card->ctx,
			       "DEE rbuf=%p,rbuflen=%"SC_FORMAT_LEN_SIZE_T"u,",
			       rbuf, rbuflen);
			body = sc_asn1_find_tag(card->ctx, rbuf, rbuflen, rbuf[0], &bodylen);
			if (body == NULL) {
				 
				 
				sc_log(card->ctx, " ***** tag 0x53 MISSING");
				r = SC_ERROR_INVALID_DATA;
				goto err;
			}
			if (bodylen > body - rbuf + rbuflen) {
				sc_log(card->ctx,
				       " ***** tag length > then data: %"SC_FORMAT_LEN_SIZE_T"u>%"SC_FORMAT_LEN_PTRDIFF_T"u+%"SC_FORMAT_LEN_SIZE_T"u",
				       bodylen, body - rbuf, rbuflen);
				r = SC_ERROR_INVALID_DATA;
				goto err;
			}
			 
			if (priv->return_only_cert || piv_objects[enumtag].flags & PIV_OBJECT_TYPE_PUBKEY) {
				r = piv_cache_internal_data(card, enumtag);
				if (r < 0)
					goto err;
			}

		}
		priv->rwb_state = 0;
	}

	if (priv->return_only_cert || piv_objects[enumtag].flags & PIV_OBJECT_TYPE_PUBKEY) {
		rbuf = priv->obj_cache[enumtag].internal_obj_data;
		rbuflen = priv->obj_cache[enumtag].internal_obj_len;
	} else {
		rbuf = priv->obj_cache[enumtag].obj_data;
		rbuflen = priv->obj_cache[enumtag].obj_len;
	}
	 

	if ( rbuflen < idx + count)
		count = rbuflen - idx;
	if (count <= 0) {
		r = 0;
		priv->rwb_state = 1;
	} else {
		memcpy(buf, rbuf + idx, count);
		r = count;
	}

err:
	LOG_FUNC_RETURN(card->ctx, r);
}