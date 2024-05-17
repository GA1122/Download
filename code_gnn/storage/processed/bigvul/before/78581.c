pgp_delete_file(sc_card_t *card, const sc_path_t *path)
{
	struct pgp_priv_data *priv = DRVDATA(card);
	pgp_blob_t *blob;
	sc_file_t *file;
	int r;

	LOG_FUNC_CALLED(card->ctx);

	 
	r = pgp_select_file(card, path, &file);
	LOG_TEST_RET(card->ctx, r, "Cannot select file.");

	 
	blob = priv->current;

	 
	if (blob == priv->mf)
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_NOT_SUPPORTED);

	if (card->type != SC_CARD_TYPE_OPENPGP_GNUK &&
		(file->id == DO_SIGN_SYM || file->id == DO_ENCR_SYM || file->id == DO_AUTH_SYM)) {
		 
		r = SC_SUCCESS;
	}
	else if (card->type == SC_CARD_TYPE_OPENPGP_GNUK && file->id == DO_SIGN_SYM) {
		r = gnuk_delete_key(card, 1);
	}
	else if (card->type == SC_CARD_TYPE_OPENPGP_GNUK && file->id == DO_ENCR_SYM) {
		r = gnuk_delete_key(card, 2);
	}
	else if (card->type == SC_CARD_TYPE_OPENPGP_GNUK && file->id == DO_AUTH_SYM) {
		r = gnuk_delete_key(card, 3);
	}
	else {
		 
		r = pgp_put_data(card, file->id, NULL, 0);
	}

	 
	priv->current = blob->parent;

	LOG_FUNC_RETURN(card->ctx, r);
}
