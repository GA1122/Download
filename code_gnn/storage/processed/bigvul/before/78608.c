pgp_select_file(sc_card_t *card, const sc_path_t *path, sc_file_t **ret)
{
	struct pgp_priv_data *priv = DRVDATA(card);
	pgp_blob_t	*blob;
	unsigned int	path_start = 0;
	unsigned int	n;
	sc_path_t dummy_path;

	LOG_FUNC_CALLED(card->ctx);

	if (path->type == SC_PATH_TYPE_DF_NAME)
		LOG_FUNC_RETURN(card->ctx, iso_ops->select_file(card, path, ret));

	if (path->len < 2 || (path->len & 1))
		LOG_TEST_RET(card->ctx, SC_ERROR_INVALID_ARGUMENTS,
				"invalid path length");

	if (path->type == SC_PATH_TYPE_FILE_ID && path->len != 2)
		LOG_TEST_RET(card->ctx, SC_ERROR_INVALID_ARGUMENTS,
				"invalid path type");

	 
	sc_format_path("11001101", &dummy_path);
	if (sc_compare_path(path, &dummy_path)) {
		if (ret != NULL) {
			*ret = sc_file_new();
			 
			(*ret)->size = priv->max_cert_size;
		}
		LOG_FUNC_RETURN(card->ctx, SC_SUCCESS);
	}

	 
	path_start = pgp_strip_path(card, path);

	 
	blob = priv->mf;
	 
	for (n = path_start; n < path->len; n += 2) {
		unsigned int	id = bebytes2ushort(path->value + n);
		int		r = pgp_get_blob(card, blob, id, &blob);

		 
		if (id == 0x4402 || id == 0x5f48) {
			if (ret == NULL)
				 
				LOG_FUNC_RETURN(card->ctx, SC_SUCCESS);

			 
			*ret = sc_file_new();
			(*ret)->size = priv->max_cert_size;
			LOG_FUNC_RETURN(card->ctx, SC_SUCCESS);
		}

		if (r < 0) {	 
			LOG_FUNC_RETURN(card->ctx, r);
		}
	}

	 
	priv->current = blob;

	if (ret)
		sc_file_dup(ret, blob->file);

	LOG_FUNC_RETURN(card->ctx, SC_SUCCESS);
}
