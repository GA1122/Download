int sc_pkcs15emu_itacns_init_ex(sc_pkcs15_card_t *p15card, struct sc_aid *aid,
		sc_pkcs15emu_opt_t *opts)
{
	sc_card_t *card = p15card->card;
	SC_FUNC_CALLED(card->ctx, 1);

	 
	if (!(opts && opts->flags & SC_PKCS15EMU_FLAGS_NO_CHECK)) {
		if (! (
				(card->type > SC_CARD_TYPE_ITACNS_BASE &&
				card->type < SC_CARD_TYPE_ITACNS_BASE + 1000)
			|| card->type == SC_CARD_TYPE_CARDOS_CIE_V1)
			)
			return SC_ERROR_WRONG_CARD;
	}

	 
	return itacns_init(p15card);
}