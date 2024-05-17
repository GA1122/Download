static int rtecp_card_ctl(sc_card_t *card, unsigned long request, void *data)
{
	sc_apdu_t apdu;
	u8 buf[SC_MAX_APDU_BUFFER_SIZE];
	sc_rtecp_genkey_data_t *genkey_data = data;
	sc_serial_number_t *serial = data;
	int r;

	assert(card && card->ctx);
	switch (request)
	{
	case SC_CARDCTL_RTECP_INIT:
		sc_format_apdu(card, &apdu, SC_APDU_CASE_1, 0x8A, 0, 0);
		apdu.cla = 0x80;
		break;
	case SC_CARDCTL_RTECP_INIT_END:
		sc_format_apdu(card, &apdu, SC_APDU_CASE_1, 0x84, 0x4E, 0x19);
		apdu.cla = 0x80;
		break;
	case SC_CARDCTL_GET_SERIALNR:
		if (!serial)
			SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_NORMAL, SC_ERROR_INVALID_ARGUMENTS);
		sc_format_apdu(card, &apdu, SC_APDU_CASE_2_SHORT, 0xCA, 0x01, 0x81);
		apdu.resp = buf;
		apdu.resplen = sizeof(buf);
		apdu.le = 256;
		serial->len = sizeof(serial->value);
		break;
	case SC_CARDCTL_RTECP_GENERATE_KEY:
		if (!genkey_data)
			SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_NORMAL, SC_ERROR_INVALID_ARGUMENTS);
		sc_format_apdu(card, &apdu, SC_APDU_CASE_2_SHORT, 0x46, 0x80,
				genkey_data->key_id);
		apdu.resp = buf;
		apdu.resplen = sizeof(buf);
		apdu.le = 256;
		break;
	case SC_CARDCTL_LIFECYCLE_SET:
		sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "%s\n",
				"SC_CARDCTL_LIFECYCLE_SET not supported");
		 
		return SC_ERROR_NOT_SUPPORTED;
	default:
		sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL,
			"request = 0x%lx\n", request);
		SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_NORMAL, SC_ERROR_NOT_SUPPORTED);
	}
	r = sc_transmit_apdu(card, &apdu);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");
	r = sc_check_sw(card, apdu.sw1, apdu.sw2);
	if (!r && request == SC_CARDCTL_RTECP_GENERATE_KEY)
	{
		if (genkey_data->type == SC_ALGORITHM_RSA &&
				genkey_data->u.rsa.modulus_len >= apdu.resplen &&
				genkey_data->u.rsa.exponent_len >= 3)
		{
			memcpy(genkey_data->u.rsa.modulus, apdu.resp, apdu.resplen);
			genkey_data->u.rsa.modulus_len = apdu.resplen;
			reverse(genkey_data->u.rsa.modulus,
					genkey_data->u.rsa.modulus_len);
			memcpy(genkey_data->u.rsa.exponent, "\x01\x00\x01", 3);
			genkey_data->u.rsa.exponent_len = 3;
		}
		else if (genkey_data->type == SC_ALGORITHM_GOSTR3410 &&
				genkey_data->u.gostr3410.xy_len >= apdu.resplen)
		{
			memcpy(genkey_data->u.gostr3410.xy, apdu.resp, apdu.resplen);
			genkey_data->u.gostr3410.xy_len = apdu.resplen;
		}
		else
			r = SC_ERROR_BUFFER_TOO_SMALL;
	}
	else if (!r && request == SC_CARDCTL_GET_SERIALNR)
	{
		if (serial->len >= apdu.resplen)
		{
			memcpy(serial->value, apdu.resp, apdu.resplen);
			serial->len = apdu.resplen;
		}
		else
			r = SC_ERROR_BUFFER_TOO_SMALL;
	}
	SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_VERBOSE, r);
}