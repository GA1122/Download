static int coolkey_match_card(sc_card_t *card)
{
	int r;
	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);
	 
	card->ops->logout = NULL;

	r = coolkey_select_applet(card);
	return (r >= SC_SUCCESS);
}
