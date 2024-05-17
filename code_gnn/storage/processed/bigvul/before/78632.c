piv_finish(sc_card_t *card)
{
	piv_private_data_t * priv = PIV_DATA(card);
	int i;

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);
	if (priv) {
		sc_file_free(priv->aid_file);
		if (priv->w_buf)
			free(priv->w_buf);
		if (priv->offCardCertURL)
			free(priv->offCardCertURL);
		for (i = 0; i < PIV_OBJ_LAST_ENUM - 1; i++) {
			sc_log(card->ctx,
			       "DEE freeing #%d, 0x%02x %p:%"SC_FORMAT_LEN_SIZE_T"u %p:%"SC_FORMAT_LEN_SIZE_T"u",
			       i, priv->obj_cache[i].flags,
			       priv->obj_cache[i].obj_data,
			       priv->obj_cache[i].obj_len,
			       priv->obj_cache[i].internal_obj_data,
			       priv->obj_cache[i].internal_obj_len);
			if (priv->obj_cache[i].obj_data)
				free(priv->obj_cache[i].obj_data);
			if (priv->obj_cache[i].internal_obj_data)
				free(priv->obj_cache[i].internal_obj_data);
		}
		free(priv);
		card->drv_data = NULL;  
	}
	return 0;
}