pgp_pin_cmd(sc_card_t *card, struct sc_pin_cmd_data *data, int *tries_left)
{
	struct pgp_priv_data *priv = DRVDATA(card);

	LOG_FUNC_CALLED(card->ctx);

	if (data->pin_type != SC_AC_CHV)
		LOG_TEST_RET(card->ctx, SC_ERROR_INVALID_ARGUMENTS,
				"invalid PIN type");

	 

	 
	data->pin_reference |= 0x80;

	 
	if (data->cmd == SC_PIN_CMD_CHANGE || data->cmd == SC_PIN_CMD_UNBLOCK) {
		if (priv->bcd_version >= OPENPGP_CARD_2_0) {
			if (data->pin_reference == 0x82)
				data->pin_reference = 0x81;

			if (data->cmd == SC_PIN_CMD_CHANGE) {
				if (data->pin1.len == 0 &&
				    !(data->flags & SC_PIN_CMD_USE_PINPAD))
					LOG_TEST_RET(card->ctx, SC_ERROR_INVALID_ARGUMENTS,
						     "v2 cards don't support implicit old PIN for PIN change.");

				data->flags &= ~SC_PIN_CMD_IMPLICIT_CHANGE;
			}
		} else {
			if (data->pin1.len != 0) {
				sc_log(card->ctx,
				       "v1 cards don't support explicit old or CHV3 PIN, PIN ignored.");
				sc_log(card->ctx,
				       "please make sure that you have verified the relevant PIN first.");
				data->pin1.len = 0;
			}

			data->flags |= SC_PIN_CMD_IMPLICIT_CHANGE;
		}
	}

	if (data->cmd == SC_PIN_CMD_UNBLOCK && data->pin2.len == 0 &&
	    !(data->flags & SC_PIN_CMD_USE_PINPAD))
		LOG_TEST_RET(card->ctx, SC_ERROR_INVALID_ARGUMENTS,
			     "new PIN must be provided for unblock operation.");

	 
	if (!(data->pin_reference == 0x81 || data->pin_reference == 0x82 || data->pin_reference == 0x83)) {
		LOG_TEST_RET(card->ctx, SC_ERROR_INVALID_ARGUMENTS,
					 "key-id should be 1, 2, 3.");
	}
	LOG_FUNC_RETURN(card->ctx, iso_ops->pin_cmd(card, data, tries_left));
}
