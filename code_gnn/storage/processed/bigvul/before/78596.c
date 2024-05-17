pgp_list_files(sc_card_t *card, u8 *buf, size_t buflen)
{
	struct pgp_priv_data *priv = DRVDATA(card);
	pgp_blob_t	*blob;
	unsigned int	k;
	int		r;

	LOG_FUNC_CALLED(card->ctx);

	 
	blob = priv->current;

	if (blob->file->type != SC_FILE_TYPE_DF)
		LOG_TEST_RET(card->ctx, SC_ERROR_OBJECT_NOT_VALID,
				"invalid file type");

	if ((r = pgp_enumerate_blob(card, blob)) < 0)
		LOG_FUNC_RETURN(card->ctx, r);

	for (k = 0, blob = blob->files; blob != NULL; blob = blob->next) {
		if (blob->info != NULL && (blob->info->access & READ_MASK) != READ_NEVER) {
			if (k + 2 > buflen)
				LOG_FUNC_RETURN(card->ctx, SC_ERROR_BUFFER_TOO_SMALL);

			ushort2bebytes(buf + k, blob->id);
			k += 2;
		}
	}

	LOG_FUNC_RETURN(card->ctx, k);
}