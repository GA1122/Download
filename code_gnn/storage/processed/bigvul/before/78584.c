pgp_finish(sc_card_t *card)
{
	if (card != NULL) {
		struct pgp_priv_data *priv = DRVDATA(card);

		if (priv != NULL) {
			 
			pgp_iterate_blobs(priv->mf, 99, pgp_free_blob);

			 
			free(priv);
		}
		card->drv_data = NULL;
	}
	return SC_SUCCESS;
}