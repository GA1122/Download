static int cac_match_card(sc_card_t *card)
{
	int r;
	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);
	 
	card->ops->logout = NULL;

	r = cac_find_and_initialize(card, 0);
	return (r == SC_SUCCESS);  
}
