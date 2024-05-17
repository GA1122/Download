pgp_read_blob(sc_card_t *card, pgp_blob_t *blob)
{
	struct pgp_priv_data *priv = DRVDATA(card);

	if (blob->data != NULL)
		return SC_SUCCESS;
	if (blob->info == NULL)
		return blob->status;

	if (blob->info->get_fn) {	 
		u8 	buffer[2048];
		size_t	buf_len = sizeof(buffer);
		int r = SC_SUCCESS;

		 
		if (blob->id == DO_CERT && priv->max_cert_size > 0) {
			buf_len = MIN(priv->max_cert_size, sizeof(buffer));
		}

		 
		if (card->type == SC_CARD_TYPE_OPENPGP_GNUK &&
		    (blob->id == DO_AUTH ||
		     blob->id == DO_SIGN ||
		     blob->id == DO_ENCR ||
		     blob->id == DO_AUTH_SYM ||
		     blob->id == DO_SIGN_SYM ||
		     blob->id == DO_ENCR_SYM)) {
			buf_len = MAXLEN_RESP_PUBKEY_GNUK;
		}

		r = blob->info->get_fn(card, blob->id, buffer, buf_len);

		if (r < 0) {	 
			blob->status = r;
			return r;
		}

		return pgp_set_blob(blob, buffer, r);
	}
	else {		 
		return SC_SUCCESS;
	}
}
