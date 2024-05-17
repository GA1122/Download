coolkey_pin_cmd(sc_card_t *card, struct sc_pin_cmd_data *data, int *tries_left)
{
	int r;
	coolkey_private_data_t * priv = COOLKEY_DATA(card);
	size_t rbuflen;
	u8 *rbuf;

	 
	switch (data->cmd) {
	case SC_PIN_CMD_GET_INFO:
		if (priv->nonce_valid) {
			data->pin1.logged_in = SC_PIN_STATE_LOGGED_IN;
		} else {
			data->pin1.logged_in = SC_PIN_STATE_LOGGED_OUT;
			 
			data->pin1.tries_left = 0xf;
		}
		if (tries_left) {
			*tries_left = data->pin1.tries_left;
		}
		r = SC_SUCCESS;
		break;

	case SC_PIN_CMD_UNBLOCK:
	case SC_PIN_CMD_CHANGE:
		 
	default:
		r = SC_ERROR_NOT_SUPPORTED;
		break;
	case SC_PIN_CMD_VERIFY:
		 
		 
		rbuflen = sizeof(priv->nonce);
		rbuf = &priv->nonce[0];
		r = coolkey_apdu_io(card, COOLKEY_CLASS, COOLKEY_INS_VERIFY_PIN,
			data->pin_reference, 0, data->pin1.data, data->pin1.len,
			&rbuf, &rbuflen, NULL, 0);
		if (r < 0) {
			break;
		}
		priv->nonce_valid = 1;
		r = SC_SUCCESS;
	}
	return r;
}
