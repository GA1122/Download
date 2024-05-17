gpk_select_file(sc_card_t *card, const sc_path_t *path,
		sc_file_t **file)
{
	unsigned short int	pathtmp[SC_MAX_PATH_SIZE/2];
	unsigned short int	*pathptr;
	size_t			pathlen, n;
	int			locked = 0, r = 0, use_relative = 0, retry = 1;
	u8			leaf_type;

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);

	 
	if (path->type == SC_PATH_TYPE_DF_NAME) {
		if (path->len > 16)
			return SC_ERROR_INVALID_ARGUMENTS;
		r = gpk_select(card, GPK_SEL_AID,
					path->value, path->len, file);
		goto done;
	}

	 
	leaf_type = GPK_SEL_EF;

try_again:
	if ((path->len & 1) || path->len > sizeof(pathtmp))
		return SC_ERROR_INVALID_ARGUMENTS;
	pathptr = pathtmp;
	for (n = 0; n < path->len; n += 2)
		pathptr[n>>1] = (path->value[n] << 8)|path->value[n+1];
	pathlen = path->len >> 1;

	 
	if (path->type == SC_PATH_TYPE_PATH) {
		 
		if ((pathptr[0] == GPK_FID_MF && pathlen > 2)
		 || (pathptr[0] != GPK_FID_MF && pathlen > 1))
			retry = 0;
		use_relative = match_path(card, &pathptr, &pathlen, file != 0);
		if (pathlen == 0)
			goto done;
	} else {
		 
		if (pathlen > 1)
			return SC_ERROR_INVALID_ARGUMENTS;
		use_relative = 1;
	}

	if (pathlen == 1 && pathptr[0] == GPK_FID_MF) {
		 
		leaf_type = GPK_SEL_MF;
	} else {
		if (!locked++) {
			r = sc_lock(card);
			SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "sc_lock() failed");
		}

		 
		if (!use_relative) {
			r = gpk_select_id(card, GPK_SEL_MF, GPK_FID_MF, NULL);
			if (r)
				sc_unlock(card);
			SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "Unable to select MF");

			 
			if (pathptr[0] == GPK_FID_MF) {
				pathptr++;
				pathlen--;
			}
			if (pathlen == 0)
				goto done;
		}

		 
		while (pathlen > 1) {
			r = gpk_select_id(card, GPK_SEL_DF, pathptr[0], NULL);
			if (r)
				sc_unlock(card);
			SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "Unable to select DF");
			pathptr++;
			pathlen--;
		}
	}

	 
	r = gpk_select_id(card, leaf_type, pathptr[0], file);
	if (r) {
		 
		if (leaf_type == GPK_SEL_EF && retry) {
			card->cache.current_path.len = 0;
			leaf_type = GPK_SEL_DF;
			goto try_again;
		}
	}

done:
	if (locked)
		sc_unlock(card);
	return r;
}
