static int piv_select_file(sc_card_t *card, const sc_path_t *in_path,
	sc_file_t **file_out)
{
	piv_private_data_t * priv = PIV_DATA(card);
	int r;
	int i;
	const u8 *path;
	int pathlen;
	sc_file_t *file = NULL;
	u8 * rbuf = NULL;
	size_t rbuflen = 0;

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);

	path = in_path->value;
	pathlen = in_path->len;

	 
	 

	if (memcmp(path, "\x3F\x00", 2) == 0) {
		if (pathlen > 2) {
			path += 2;
			pathlen -= 2;
		}
	}

	i = piv_find_obj_by_containerid(card, path);

	if (i < 0)
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_FILE_NOT_FOUND);

	 
	priv->return_only_cert = (pathlen == 4 && path[2] == 0xce && path[3] == 0xce);

	priv->selected_obj = i;
	priv->rwb_state = -1;

	 
	 

	if (file_out) {
		 
		r = piv_get_cached_data(card, i, &rbuf, &rbuflen);

		if (r < 0)
			LOG_FUNC_RETURN(card->ctx, SC_ERROR_FILE_NOT_FOUND);

		 
		if (priv->return_only_cert || piv_objects[i].flags & PIV_OBJECT_TYPE_PUBKEY) {
			r = piv_cache_internal_data(card, i);
			if (r < 0)
				LOG_FUNC_RETURN(card->ctx, r);
		}

		file = sc_file_new();
		if (file == NULL)
			LOG_FUNC_RETURN(card->ctx, SC_ERROR_OUT_OF_MEMORY);

		file->path = *in_path;
		 
		file->type =  SC_FILE_TYPE_DF;
		file->shareable = 0;
		file->ef_structure = 0;
		if (priv->return_only_cert)
			file->size = priv->obj_cache[i].internal_obj_len;
		else
			file->size = priv->obj_cache[i].obj_len;

		file->id = (piv_objects[i].containerid[0]<<8) + piv_objects[i].containerid[1];

		*file_out = file;
	}

	LOG_FUNC_RETURN(card->ctx, SC_SUCCESS);

}
