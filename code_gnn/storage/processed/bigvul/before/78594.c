pgp_init(sc_card_t *card)
{
	struct pgp_priv_data *priv;
	sc_path_t	path;
	sc_file_t	*file = NULL;
	struct do_info	*info;
	int		r;

	LOG_FUNC_CALLED(card->ctx);

	priv = calloc (1, sizeof *priv);
	if (!priv)
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_OUT_OF_MEMORY);
	card->drv_data = priv;

	card->cla = 0x00;

	 
	sc_format_path("D276:0001:2401", &path);
	path.type = SC_PATH_TYPE_DF_NAME;
	if ((r = iso_ops->select_file(card, &path, &file)) < 0) {
		pgp_finish(card);
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_INVALID_CARD);
	}

	 
	if (!file)   {
		pgp_finish(card);
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_INVALID_CARD);
	}

	if (file->namelen != 16) {
		 
		r = get_full_pgp_aid(card, file);
		if (r < 0) {
			pgp_finish(card);
			LOG_FUNC_RETURN(card->ctx, SC_ERROR_INVALID_CARD);
		}
	}

	 
	if (file->namelen == 16) {
		static char card_name[SC_MAX_APDU_BUFFER_SIZE] = "OpenPGP card";

		 
		priv->bcd_version = bebytes2ushort(file->name + 6);
		card->version.fw_major = card->version.hw_major = BCD2UCHAR(file->name[6]);
		card->version.fw_minor = card->version.hw_minor = BCD2UCHAR(file->name[7]);

		 
		if (card->name == default_cardname_v1 ||
		    card->name == default_cardname_v2 ||
		    card->name == default_cardname_v3) {
			snprintf(card_name, sizeof(card_name),
				 "OpenPGP card v%u.%u (%04X %08lX)",
				 card->version.hw_major, card->version.hw_minor,
				 bebytes2ushort(file->name + 8),
				 bebytes2ulong(file->name + 10));
		}
		else if (card->name != NULL) {
			 
			snprintf(card_name, sizeof(card_name),
				 "%s (%04X %08lX)",
				 card->name,
				 bebytes2ushort(file->name + 8),
				 bebytes2ulong(file->name + 10));
		}
		card->name = card_name;

		 
		memcpy(card->serialnr.value, file->name + 8, 6);
		card->serialnr.len = 6;
	} else {
		 
		switch (card->type) {
			case SC_CARD_TYPE_OPENPGP_V3:
				priv->bcd_version = OPENPGP_CARD_3_0;
				break;
			case SC_CARD_TYPE_OPENPGP_GNUK:
			case SC_CARD_TYPE_OPENPGP_V2:
				priv->bcd_version = OPENPGP_CARD_2_0;
				break;
			default:
				priv->bcd_version = OPENPGP_CARD_1_1;
				break;
		}
	}

	 
	priv->pgp_objects = (priv->bcd_version < OPENPGP_CARD_2_0) ? pgp1x_objects
			  : (priv->bcd_version < OPENPGP_CARD_2_1) ? pgp20_objects
			  : (priv->bcd_version < OPENPGP_CARD_3_0) ? pgp21_objects
			  : (priv->bcd_version < OPENPGP_CARD_3_3) ? pgp30_objects
			  :					     pgp33_objects;

	 
	sc_format_path("3f00", &file->path);

	 
	priv->mf = pgp_new_blob(card, NULL, 0x3f00, file);
	if (!priv->mf) {
		pgp_finish(card);
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_OUT_OF_MEMORY);
	}

	 
	priv->current = priv->mf;

	 
	for (info = priv->pgp_objects; (info != NULL) && (info->id > 0); info++) {
		if (((info->access & READ_MASK) != READ_NEVER) && (info->get_fn != NULL)) {
			pgp_blob_t *child = NULL;

			child = pgp_new_blob(card, priv->mf, info->id, sc_file_new());

			 
			if (child == NULL) {
				pgp_finish(card);
				LOG_FUNC_RETURN(card->ctx, SC_ERROR_OUT_OF_MEMORY);
			}
		}
	}

	 
	pgp_get_card_features(card);

	LOG_FUNC_RETURN(card->ctx, SC_SUCCESS);
}