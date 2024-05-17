pgp_read_binary(sc_card_t *card, unsigned int idx,
		u8 *buf, size_t count, unsigned long flags)
{
	struct pgp_priv_data *priv = DRVDATA(card);
	pgp_blob_t	*blob;
	int		r;

	LOG_FUNC_CALLED(card->ctx);

	 
	blob = priv->current;

	if (blob == NULL)
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_FILE_NOT_FOUND);

	if (blob->file->type != SC_FILE_TYPE_WORKING_EF)
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_FILE_NOT_FOUND);

	if ((r = pgp_read_blob(card, blob)) < 0)
		LOG_FUNC_RETURN(card->ctx, r);

	if (idx > blob->len)
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_INCORRECT_PARAMETERS);

	if (idx + count > blob->len)
		count = blob->len - idx;
	memcpy(buf, blob->data + idx, count);

	LOG_FUNC_RETURN(card->ctx, (int)count);
}