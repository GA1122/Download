static int entersafe_init(sc_card_t *card)
{
	unsigned int flags;

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);

	card->name = "entersafe";
	card->cla  = 0x00;
	card->drv_data = NULL;

	flags =SC_ALGORITHM_ONBOARD_KEY_GEN
		 | SC_ALGORITHM_RSA_RAW
		 | SC_ALGORITHM_RSA_HASH_NONE;

	_sc_card_add_rsa_alg(card, 512, flags, 0);
	_sc_card_add_rsa_alg(card, 768, flags, 0);
	_sc_card_add_rsa_alg(card,1024, flags, 0);
	_sc_card_add_rsa_alg(card,2048, flags, 0);

	card->caps = SC_CARD_CAP_RNG; 

	 
	card->max_send_size = 224;
	card->max_recv_size = 224;
	SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_VERBOSE,SC_SUCCESS);
}