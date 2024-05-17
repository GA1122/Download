auth_pin_verify_pinpad(struct sc_card *card, int pin_reference, int *tries_left)
{
	struct sc_card_driver *iso_drv = sc_get_iso7816_driver();
	struct sc_pin_cmd_data pin_cmd;
	struct sc_apdu apdu;
	unsigned char ffs1[0x100];
	int rv;

	LOG_FUNC_CALLED(card->ctx);

	memset(ffs1, 0xFF, sizeof(ffs1));
	memset(&pin_cmd, 0, sizeof(pin_cmd));

        rv = auth_pin_is_verified(card, pin_reference, tries_left);
    	sc_log(card->ctx, "auth_pin_is_verified returned rv %i", rv);

	 
	if (!rv)
		LOG_FUNC_RETURN(card->ctx, rv);

	pin_cmd.flags |= SC_PIN_CMD_NEED_PADDING;

	 
	sc_format_apdu(card, &apdu, SC_APDU_CASE_3_SHORT, 0x20, 0x00, pin_reference);
	apdu.lc = OBERTHUR_AUTH_MAX_LENGTH_PIN;
	apdu.datalen = OBERTHUR_AUTH_MAX_LENGTH_PIN;
	apdu.data = ffs1;

	pin_cmd.apdu = &apdu;
	pin_cmd.pin_type = SC_AC_CHV;
	pin_cmd.cmd = SC_PIN_CMD_VERIFY;
	pin_cmd.flags |= SC_PIN_CMD_USE_PINPAD;
	pin_cmd.pin_reference = pin_reference;
	if (pin_cmd.pin1.min_length < 4)
		pin_cmd.pin1.min_length = 4;
	pin_cmd.pin1.max_length = 8;
	pin_cmd.pin1.encoding = SC_PIN_ENCODING_ASCII;
	pin_cmd.pin1.offset = 5;
	pin_cmd.pin1.data = ffs1;
	pin_cmd.pin1.len = OBERTHUR_AUTH_MAX_LENGTH_PIN;
	pin_cmd.pin1.pad_length = OBERTHUR_AUTH_MAX_LENGTH_PIN;

	rv = iso_drv->ops->pin_cmd(card, &pin_cmd, tries_left);
	LOG_TEST_RET(card->ctx, rv, "PIN CMD 'VERIFY' with pinpad failed");

	LOG_FUNC_RETURN(card->ctx, rv);
}