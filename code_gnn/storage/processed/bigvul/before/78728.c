int sc_pkcs15emu_tcos_init_ex(
	sc_pkcs15_card_t   *p15card,
	struct sc_aid *aid,
	sc_pkcs15emu_opt_t *opts
){
	sc_card_t         *card = p15card->card;
	sc_context_t      *ctx = p15card->card->ctx;
	sc_serial_number_t serialnr;
	char               serial[30];
	int i, r;

	 
	i=(opts && (opts->flags & SC_PKCS15EMU_FLAGS_NO_CHECK));
	if (!i && card->type!=SC_CARD_TYPE_TCOS_V2 && card->type!=SC_CARD_TYPE_TCOS_V3) return SC_ERROR_WRONG_CARD;

	 
	r = sc_card_ctl(card, SC_CARDCTL_GET_SERIALNR, &serialnr);
	if (r < 0) {
		sc_debug(ctx, SC_LOG_DEBUG_NORMAL, "unable to get ICCSN\n");
		return SC_ERROR_WRONG_CARD;
	}
        sc_bin_to_hex(serialnr.value, serialnr.len , serial, sizeof(serial), 0);
	serial[19] = '\0';
        p15card->tokeninfo->serial_number = strdup(serial);

	if(!detect_netkey(p15card)) return SC_SUCCESS;
	if(!detect_idkey(p15card)) return SC_SUCCESS;
	if(!detect_unicard(p15card)) return SC_SUCCESS;
	if(!detect_signtrust(p15card)) return SC_SUCCESS;
	if(!detect_datev(p15card)) return SC_SUCCESS;

	return SC_ERROR_INTERNAL;
}