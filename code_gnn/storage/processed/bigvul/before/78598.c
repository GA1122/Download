pgp_match_card(sc_card_t *card)
{
	int i;

	LOG_FUNC_CALLED(card->ctx);

	i = _sc_match_atr(card, pgp_atrs, &card->type);
	if (i >= 0) {
		card->name = pgp_atrs[i].name;
		LOG_FUNC_RETURN(card->ctx, 1);
	}
	else {
		sc_path_t	partial_aid;
		sc_file_t *file = NULL;

		 
		sc_format_path("D276:0001:2401", &partial_aid);
		partial_aid.type = SC_PATH_TYPE_DF_NAME;
		 
		i = iso_ops->select_file(card, &partial_aid, &file);
		if (SC_SUCCESS == i) {
			card->type = SC_CARD_TYPE_OPENPGP_BASE;
			card->name = default_cardname;

			if (file->namelen != 16)
				(void) get_full_pgp_aid(card, file);
			if (file->namelen == 16) {
				unsigned char major = BCD2UCHAR(file->name[6]);

				switch (major) {
					case 1:
						card->type = SC_CARD_TYPE_OPENPGP_V1;
						card->name = default_cardname_v1;
						break;
					case 2:
						card->type = SC_CARD_TYPE_OPENPGP_V2;
						card->name = default_cardname_v2;
						break;
					case 3:
						card->type = SC_CARD_TYPE_OPENPGP_V3;
						card->name = default_cardname_v3;
						break;
					default:
						break;
				}
			}
			sc_file_free(file);
			LOG_FUNC_RETURN(card->ctx, 1);
		}
	}
	LOG_FUNC_RETURN(card->ctx, 0);
}