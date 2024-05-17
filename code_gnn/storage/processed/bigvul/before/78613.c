pgp_update_binary(sc_card_t *card, unsigned int idx,
		  const u8 *buf, size_t count, unsigned long flags)
{
	struct pgp_priv_data *priv = DRVDATA(card);
	pgp_blob_t *blob = priv->current;
	int r = SC_SUCCESS;

	LOG_FUNC_CALLED(card->ctx);

	 
	if (idx > 0)
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_INCORRECT_PARAMETERS);

	 
	if (blob != NULL) {
		r = pgp_put_data(card, blob->id, buf, count);
	}

	LOG_FUNC_RETURN(card->ctx, r);
}
