pgp_put_data(sc_card_t *card, unsigned int tag, const u8 *buf, size_t buf_len)
{
	struct pgp_priv_data *priv = DRVDATA(card);
	pgp_blob_t *affected_blob = NULL;
	struct do_info *dinfo = NULL;
	int r;

	LOG_FUNC_CALLED(card->ctx);

	 
	if (priv->current->id != tag)
		affected_blob = pgp_find_blob(card, tag);

	 
	if (affected_blob == NULL)
		dinfo = pgp_get_info_by_tag(card, tag);
	else
		dinfo = affected_blob->info;

	if (dinfo == NULL) {
		sc_log(card->ctx, "The DO %04X does not exist.", tag);
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_INVALID_ARGUMENTS);
	}
	else if ((dinfo->access & WRITE_MASK) == WRITE_NEVER) {
		sc_log(card->ctx, "DO %04X is not writable.", tag);
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_NOT_ALLOWED);
	}

	 
	if (tag == DO_CERT && buf_len > priv->max_cert_size) {
		sc_log(card->ctx,
		       "Data size %"SC_FORMAT_LEN_SIZE_T"u exceeds DO size limit %"SC_FORMAT_LEN_SIZE_T"u.",
		       buf_len, priv->max_cert_size);
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_WRONG_LENGTH);
	}

	if (tag == DO_CERT && card->type == SC_CARD_TYPE_OPENPGP_GNUK) {
		 
		r = gnuk_write_certificate(card, buf, buf_len);
	}
	else {
		r = pgp_put_data_plain(card, tag, buf, buf_len);
	}

	 
	if (r == SC_ERROR_SECURITY_STATUS_NOT_SATISFIED) {
		sc_debug(card->ctx, SC_LOG_DEBUG_VERBOSE, "Please verify PIN first.");
	}
	LOG_TEST_RET(card->ctx, r, "PUT DATA returned error");

	if (affected_blob) {
		 
		sc_log(card->ctx, "Updating the corresponding blob data");
		r = pgp_set_blob(affected_blob, buf, buf_len);
		if (r < 0)
			sc_log(card->ctx, "Failed to update blob %04X. Error %d.", affected_blob->id, r);
		 
	}

	LOG_FUNC_RETURN(card->ctx, (int)buf_len);
}
