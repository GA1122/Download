auth_pin_reset(struct sc_card *card, unsigned int type,
		struct sc_pin_cmd_data *data, int *tries_left)
{
	int rv;

	LOG_FUNC_CALLED(card->ctx);

	 
	rv = auth_pin_reset_oberthur_style(card, SC_AC_CHV, data, tries_left);
	LOG_TEST_RET(card->ctx, rv, "Oberthur style 'PIN RESET' failed");

	LOG_FUNC_RETURN(card->ctx, rv);
}