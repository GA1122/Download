pgp_parse_hist_bytes(sc_card_t *card, u8 *ctlv, size_t ctlv_len)
{
	struct pgp_priv_data *priv = DRVDATA(card);
	const u8 *ptr;

	 
	if ((ptr = sc_compacttlv_find_tag(ctlv, ctlv_len, 0x73, NULL)) != NULL) {
		 
		if (ptr[2] & 0x40) {
			card->caps |= SC_CARD_CAP_APDU_EXT;
			priv->ext_caps |= EXT_CAP_APDU_EXT;
		}
		 
		if ((ptr[2] & 0x80) &&
		    (priv->bcd_version >= OPENPGP_CARD_3_0)) {
			priv->ext_caps |= EXT_CAP_CHAINING;
		}
	}

	if ((priv->bcd_version >= OPENPGP_CARD_3_0) &&
	    ((ptr = sc_compacttlv_find_tag(ctlv, ctlv_len, 0x31, NULL)) != NULL)) {
	}
}
