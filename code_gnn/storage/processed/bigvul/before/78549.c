auth_pin_change(struct sc_card *card, unsigned int type,
		struct sc_pin_cmd_data *data, int *tries_left)
{
	struct sc_card_driver *iso_drv = sc_get_iso7816_driver();
	int rv = SC_ERROR_INTERNAL;

	LOG_FUNC_CALLED(card->ctx);

	if (data->pin1.len && data->pin2.len)   {
		 
		data->flags |= SC_PIN_CMD_NEED_PADDING;
		data->flags &= ~SC_PIN_CMD_USE_PINPAD;
		data->apdu = NULL;

		data->pin_reference &= ~OBERTHUR_PIN_LOCAL;

		auth_init_pin_info(card, &data->pin1, OBERTHUR_AUTH_TYPE_PIN);
		auth_init_pin_info(card, &data->pin2, OBERTHUR_AUTH_TYPE_PIN);

		rv = iso_drv->ops->pin_cmd(card, data, tries_left);
		LOG_TEST_RET(card->ctx, rv, "CMD 'PIN CHANGE' failed");
	}
	else if (!data->pin1.len && !data->pin2.len)   {
		 
		rv = auth_pin_change_pinpad(card, data, tries_left);
		LOG_TEST_RET(card->ctx, rv, "'PIN CHANGE' failed: SOPIN verify with pinpad failed");
	}
	else   {
		LOG_TEST_RET(card->ctx, SC_ERROR_INVALID_ARGUMENTS, "'PIN CHANGE' failed");
	}

	LOG_FUNC_RETURN(card->ctx, rv);
}