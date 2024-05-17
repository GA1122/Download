pgp_get_blob(sc_card_t *card, pgp_blob_t *blob, unsigned int id,
		pgp_blob_t **ret)
{
	pgp_blob_t		*child;
	int			r;

	if ((r = pgp_enumerate_blob(card, blob)) < 0)
		return r;

	for (child = blob->files; child; child = child->next) {
		if (child->id == id) {
			(void) pgp_read_blob(card, child);
			*ret = child;
			return SC_SUCCESS;
		}
	}

	 

	 
	if (blob->id == DO_CARDHOLDER && (id == DO_NAME || id == DO_LANG_PREF || id == DO_SEX)) {
		sc_log(card->ctx, "Create blob %X under %X", id, blob->id);
		child = pgp_new_blob(card, blob, id, sc_file_new());
		if (child) {
			pgp_set_blob(child, NULL, 0);
			*ret = child;
			return SC_SUCCESS;
		}
		else
			sc_log(card->ctx,
			       "Not enough memory to create blob for DO %X",
			       id);
	}

	return SC_ERROR_FILE_NOT_FOUND;
}