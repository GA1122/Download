get_full_pgp_aid(sc_card_t *card, sc_file_t *file)
{
	int r = SC_ERROR_INVALID_ARGUMENTS;

	if (file != NULL) {
		 
		r = sc_get_data(card, 0x004F, file->name, sizeof file->name);
		file->namelen = MAX(r, 0);
	}

	return r;
}