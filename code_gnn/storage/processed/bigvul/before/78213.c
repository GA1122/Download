authentic_sm_acl_init (struct sc_card *card, struct sm_info *sm_info, int cmd,
		unsigned char *resp, size_t *resp_len)
{
	struct sc_context *ctx;
	struct sm_type_params_gp *params_gp;
	struct sc_remote_data rdata;
	int rv;

	if (!card || !sm_info || !resp || !resp_len)
		return SC_ERROR_INVALID_ARGUMENTS;

	ctx = card->ctx;
	params_gp = &sm_info->session.gp.params;

	if (!card->sm_ctx.module.ops.initialize || !card->sm_ctx.module.ops.get_apdus)
		LOG_FUNC_RETURN(ctx, SC_ERROR_NOT_SUPPORTED);

	if (*resp_len < 28)
		LOG_FUNC_RETURN(ctx, SC_ERROR_INVALID_ARGUMENTS);

	sm_info->cmd = cmd;
	sm_info->sm_type = SM_TYPE_GP_SCP01;
	sm_info->card_type = card->type;
	params_gp->index = 0;	 
	params_gp->version = 1;
	params_gp->level = 3;	 

	sm_info->serialnr = card->serialnr;

	sc_remote_data_init(&rdata);

	rv = card->sm_ctx.module.ops.initialize(ctx, sm_info, &rdata);
	LOG_TEST_RET(ctx, rv, "SM: INITIALIZE failed");
	if (!rdata.length)
		LOG_FUNC_RETURN(ctx, SC_ERROR_INTERNAL);

	rv = sc_transmit_apdu(card, &rdata.data->apdu);
	LOG_TEST_RET(ctx, rv, "transmit APDU failed");
	rv = sc_check_sw(card, rdata.data->apdu.sw1, rdata.data->apdu.sw2);
	LOG_TEST_RET(ctx, rv, "Card returned error");

	if (rdata.data->apdu.resplen != 28 || *resp_len < 28)
		LOG_FUNC_RETURN(ctx, SC_ERROR_INTERNAL);

	memcpy(resp, rdata.data->apdu.resp, 28);
	*resp_len = 28;

	rdata.free(&rdata);
	LOG_FUNC_RETURN(ctx, rv);
}