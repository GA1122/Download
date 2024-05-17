pgp_find_blob(sc_card_t *card, unsigned int tag)
{
	struct pgp_priv_data *priv = DRVDATA(card);
	pgp_blob_t *blob = NULL;
	int r;

	 
	if (priv->current->id == tag) {
		return priv->current;
	}
	 
	r = pgp_seek_blob(card, priv->mf, tag, &blob);
	if (r < 0) {
		sc_log(card->ctx, "Failed to seek the blob representing the tag %04X. Error %d.", tag, r);
		return NULL;
	}
	return blob;
}
