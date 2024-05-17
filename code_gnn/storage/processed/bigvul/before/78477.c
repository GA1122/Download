iasecc_chv_verify_pinpad(struct sc_card *card, struct sc_pin_cmd_data *pin_cmd, int *tries_left)
{
	struct sc_context *ctx = card->ctx;
	unsigned char buffer[0x100];
	int rv;

	LOG_FUNC_CALLED(ctx);
	sc_log(ctx, "CHV PINPAD PIN reference %i", pin_cmd->pin_reference);

	rv = iasecc_pin_is_verified(card, pin_cmd, tries_left);
	if (!rv)
		LOG_FUNC_RETURN(ctx, rv);

	if (!card->reader || !card->reader->ops || !card->reader->ops->perform_verify)   {
		sc_log(ctx, "Reader not ready for PIN PAD");
		LOG_FUNC_RETURN(ctx, SC_ERROR_READER);
	}

	 
	pin_cmd->pin1.len = pin_cmd->pin1.stored_length;
	pin_cmd->pin1.length_offset = 5;

	memset(buffer, 0xFF, sizeof(buffer));
	pin_cmd->pin1.data = buffer;

	pin_cmd->cmd = SC_PIN_CMD_VERIFY;
	pin_cmd->flags |= SC_PIN_CMD_USE_PINPAD;

	 

	rv = iso_ops->pin_cmd(card, pin_cmd, tries_left);
	sc_log(ctx, "rv %i", rv);

	LOG_FUNC_RETURN(ctx, rv);
}
