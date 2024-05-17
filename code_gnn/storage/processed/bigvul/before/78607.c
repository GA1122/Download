pgp_seek_blob(sc_card_t *card, pgp_blob_t *root, unsigned int id,
		pgp_blob_t **ret)
{
	pgp_blob_t	*child;
	int			r;

	if ((r = pgp_get_blob(card, root, id, ret)) == 0)
		 
		return r;

	 
	for (child = root->files; child; child = child->next) {
		 
		if ((child->info && child->info->type == SIMPLE) || child->id == DO_CERT)
			continue;
		r = pgp_seek_blob(card, child, id, ret);
		if (r == 0)
			return r;
	}

	return SC_ERROR_FILE_NOT_FOUND;
}
