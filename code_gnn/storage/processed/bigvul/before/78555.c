auth_pin_verify(struct sc_card *card, unsigned int type,
		struct sc_pin_cmd_data *data, int *tries_left)
{
	struct sc_card_driver *iso_drv = sc_get_iso7816_driver();
	int rv;

	LOG_FUNC_CALLED(card->ctx);

	if (type != SC_AC_CHV)
		LOG_TEST_RET(card->ctx, SC_ERROR_NOT_SUPPORTED, "PIN type other then SC_AC_CHV is not supported");

	data->flags |= SC_PIN_CMD_NEED_PADDING;

	auth_init_pin_info(card, &data->pin1, OBERTHUR_AUTH_TYPE_PIN);

	 
	if (data->pin_reference == OBERTHUR_PIN_REFERENCE_USER
			|| data->pin_reference == OBERTHUR_PIN_REFERENCE_ONETIME)
		data->pin_reference  |= OBERTHUR_PIN_LOCAL;

        rv = auth_pin_is_verified(card, data->pin_reference, tries_left);
    	sc_log(card->ctx, "auth_pin_is_verified returned rv %i", rv);

	 
	if (data->pin1.data && !data->pin1.len)
		LOG_FUNC_RETURN(card->ctx, rv);

	 
	if (!rv && !data->pin1.data && !data->pin1.len)
		LOG_FUNC_RETURN(card->ctx, rv);

	if (!data->pin1.data && !data->pin1.len)
		rv = auth_pin_verify_pinpad(card, data->pin_reference, tries_left);
	else
		rv = iso_drv->ops->pin_cmd(card, data, tries_left);

	LOG_FUNC_RETURN(card->ctx, rv);
}