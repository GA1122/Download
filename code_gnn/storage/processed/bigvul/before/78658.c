static int piv_write_binary(sc_card_t *card, unsigned int idx,
		const u8 *buf, size_t count, unsigned long flags)
{
	piv_private_data_t * priv = PIV_DATA(card);
	int r;
	int enumtag;

	LOG_FUNC_CALLED(card->ctx);

	if (priv->selected_obj < 0)
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_INTERNAL);

	enumtag = piv_objects[priv->selected_obj].enumtag;

	if (priv->rwb_state == 1)   
		LOG_FUNC_RETURN(card->ctx, SC_SUCCESS);

	if (priv->rwb_state == -1) {

		 
		if (priv->obj_cache[enumtag].flags & PIV_OBJ_CACHE_VALID) {
			priv->obj_cache[enumtag].flags = 0;
			if (priv->obj_cache[enumtag].obj_data) {
				free(priv->obj_cache[enumtag].obj_data);
				priv->obj_cache[enumtag].obj_data = NULL;
				priv->obj_cache[enumtag].obj_len = 0;
			}
			if (priv->obj_cache[enumtag].internal_obj_data) {
				free(priv->obj_cache[enumtag].internal_obj_data);
				priv->obj_cache[enumtag].internal_obj_data = NULL;
				priv->obj_cache[enumtag].internal_obj_len = 0;
			}
		}

		if (idx != 0)
			LOG_FUNC_RETURN(card->ctx, SC_ERROR_NO_CARD_SUPPORT);

		priv->w_buf_len = flags>>8;
		if (priv->w_buf_len == 0)
			LOG_FUNC_RETURN(card->ctx, SC_ERROR_INTERNAL);

		priv->w_buf = malloc(priv->w_buf_len);
		priv-> rwb_state = 0;
	}

	 
	if (priv->w_buf == NULL)
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_OUT_OF_MEMORY);

	if (idx + count > priv->w_buf_len)
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_OBJECT_NOT_VALID);

	memcpy(priv->w_buf + idx, buf, count);  

	 
	if (idx + count < priv->w_buf_len)
		LOG_FUNC_RETURN(card->ctx, count);

	priv-> rwb_state = 1;  

	switch (flags & 0x0f) {
		case 1:
			r = piv_write_certificate(card, priv->w_buf, priv->w_buf_len, flags & 0x10);
			break;
		case 2:  
		 
			r = priv->w_buf_len;
			break;
		default:
			r = piv_put_data(card, enumtag, priv->w_buf, priv->w_buf_len);
			break;
	}
	 
	if (r >= 0 && priv->w_buf) {
		priv->obj_cache[enumtag].flags |= PIV_OBJ_CACHE_VALID;
		priv->obj_cache[enumtag].obj_data = priv->w_buf;
		priv->obj_cache[enumtag].obj_len = priv->w_buf_len;
	} else {
		if (priv->w_buf)
			free(priv->w_buf);
	}
	priv->w_buf = NULL;
	priv->w_buf_len = 0;
	LOG_FUNC_RETURN(card->ctx, (r < 0)? r : (int)count);
}