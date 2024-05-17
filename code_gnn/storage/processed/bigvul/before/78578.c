static int pgp_card_reader_lock_obtained(sc_card_t *card, int was_reset)
{
	struct pgp_priv_data *priv = DRVDATA(card);  
	int r = SC_SUCCESS;

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);

	if (card->flags & SC_CARD_FLAG_KEEP_ALIVE
			&& was_reset <= 0
			&& priv != NULL && priv->mf && priv->mf->file) {
		 
		unsigned char aid[16];

		r = sc_get_data(card, 0x004F, aid, sizeof aid);
		if ((size_t) r != priv->mf->file->namelen
				|| 0 != memcmp(aid, priv->mf->file->name, r)) {
			 
			was_reset = 1;
		}
		r = SC_SUCCESS;
	}

	if (was_reset > 0) {
		sc_file_t	*file = NULL;
		sc_path_t	path;
		 
		sc_format_path("D276:0001:2401", &path);
		path.type = SC_PATH_TYPE_DF_NAME;
		r = iso_ops->select_file(card, &path, &file);
		sc_file_free(file);
	}

	LOG_FUNC_RETURN(card->ctx, r);
}