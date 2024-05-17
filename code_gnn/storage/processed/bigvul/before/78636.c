piv_get_cached_data(sc_card_t * card, int enumtag, u8 **buf, size_t *buf_len)
{

	piv_private_data_t * priv = PIV_DATA(card);
	int r;
	u8 *rbuf = NULL;
	size_t rbuflen;

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);
	sc_log(card->ctx, "#%d", enumtag);

	assert(enumtag >= 0 && enumtag < PIV_OBJ_LAST_ENUM);

	 
	if (priv->obj_cache[enumtag].flags & PIV_OBJ_CACHE_VALID) {

		sc_log(card->ctx,
		       "found #%d %p:%"SC_FORMAT_LEN_SIZE_T"u %p:%"SC_FORMAT_LEN_SIZE_T"u",
		       enumtag,
		       priv->obj_cache[enumtag].obj_data,
		       priv->obj_cache[enumtag].obj_len,
		       priv->obj_cache[enumtag].internal_obj_data,
		       priv->obj_cache[enumtag].internal_obj_len);


		if (priv->obj_cache[enumtag].obj_len == 0) {
			r = SC_ERROR_FILE_NOT_FOUND;
			sc_log(card->ctx, "#%d found but len=0", enumtag);
			goto err;
		}
		*buf = priv->obj_cache[enumtag].obj_data;
		*buf_len = priv->obj_cache[enumtag].obj_len;
		r = *buf_len;
		goto ok;
	}

	 

	if (priv->obj_cache[enumtag].flags & PIV_OBJ_CACHE_NOT_PRESENT) {
		sc_log(card->ctx, "no_obj #%d", enumtag);
		r = SC_ERROR_FILE_NOT_FOUND;
		goto err;
	}

	 
	sc_log(card->ctx, "get #%d",  enumtag);
	rbuflen = 1;
	r = piv_get_data(card, enumtag, &rbuf, &rbuflen);
	if (r > 0) {
		priv->obj_cache[enumtag].flags |= PIV_OBJ_CACHE_VALID;
		priv->obj_cache[enumtag].obj_len = r;
		priv->obj_cache[enumtag].obj_data = rbuf;
		*buf = rbuf;
		*buf_len = r;

		sc_log(card->ctx,
		       "added #%d  %p:%"SC_FORMAT_LEN_SIZE_T"u %p:%"SC_FORMAT_LEN_SIZE_T"u",
		       enumtag,
		       priv->obj_cache[enumtag].obj_data,
		       priv->obj_cache[enumtag].obj_len,
		       priv->obj_cache[enumtag].internal_obj_data,
		       priv->obj_cache[enumtag].internal_obj_len);

	} else if (r == 0 || r == SC_ERROR_FILE_NOT_FOUND) {
		r = SC_ERROR_FILE_NOT_FOUND;
		priv->obj_cache[enumtag].flags |= PIV_OBJ_CACHE_VALID;
		priv->obj_cache[enumtag].obj_len = 0;
	} else if ( r < 0) {
		goto err;
	}
ok:

err:
	LOG_FUNC_RETURN(card->ctx, r);
}