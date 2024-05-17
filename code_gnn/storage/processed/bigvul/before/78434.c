gpk_build_pin_apdu(sc_card_t *card, sc_apdu_t *apdu, struct sc_pin_cmd_data *data)
{
	static u8	sbuf[8];
	int		r;

	if (data->pin_type != SC_AC_CHV)
		return SC_ERROR_INVALID_ARGUMENTS;

	 
	memset(apdu, 0, sizeof(*apdu));
	apdu->cse	= SC_APDU_CASE_3_SHORT;

	data->flags |= SC_PIN_CMD_NEED_PADDING;

	switch (data->cmd) {
	case SC_PIN_CMD_VERIFY:
		 
		data->pin1.encoding = SC_PIN_ENCODING_ASCII;
		data->pin1.pad_length = 8;
		data->pin1.pad_char = 0x00;
		data->pin1.offset = 5;
		r = sc_build_pin(sbuf, 8, &data->pin1, 1);
		if (r < 0)
			return r;

		apdu->cla = 0x00;
		apdu->ins = 0x20;
		apdu->p1  = 0x00;
		break;
	case SC_PIN_CMD_CHANGE:
	case SC_PIN_CMD_UNBLOCK:
		 
		data->pin1.encoding = SC_PIN_ENCODING_BCD;
		data->pin1.pad_length = 8;
		data->pin1.pad_char = 0x00;
		data->pin1.offset = 5;
		data->pin2.encoding = SC_PIN_ENCODING_BCD;
		data->pin2.pad_length = 8;
		data->pin2.pad_char = 0x00;
		data->pin2.offset = 5 + 4;
		if ((r = sc_build_pin(sbuf, 4, &data->pin1, 1)) < 0
		 || (r = sc_build_pin(sbuf + 4, 4, &data->pin2, 1)) < 0)
			return r;

		apdu->cla = 0x80;
		apdu->ins = 0x24;
		apdu->p1  = (data->cmd == SC_PIN_CMD_CHANGE)? 0x00 : 0x01;
		break;
	default:
		return SC_ERROR_NOT_SUPPORTED;
	}

	apdu->p2	= data->pin_reference & 7;
	apdu->lc	= 8;
	apdu->datalen	= 8;
	apdu->data	= sbuf;

	return 0;
}