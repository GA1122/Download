static int asepcos_pin_cmd(sc_card_t *card, struct sc_pin_cmd_data *pdata,
	int *tries_left)
{
	sc_apdu_t apdu;
	int r = SC_SUCCESS;
	u8  sbuf[SC_MAX_APDU_BUFFER_SIZE];

	if (tries_left)
		*tries_left = -1;

	 

	 
	if (pdata->pin1.len < 4 || pdata->pin1.len > 16) {
		sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "invalid PIN1 length");
		return SC_ERROR_INVALID_PIN_LENGTH; 
	}

	switch (pdata->cmd) {
	case SC_PIN_CMD_VERIFY:
		if (pdata->pin_type != SC_AC_CHV && pdata->pin_type != SC_AC_AUT)
			return SC_ERROR_INVALID_ARGUMENTS;
		 
		if (pdata->pin_type == SC_AC_AUT && pdata->pin_reference)
			return SC_ERROR_INVALID_ARGUMENTS;
		 
		r = asepcos_build_pin_apdu(card, &apdu, pdata, sbuf, sizeof(sbuf), SC_PIN_CMD_VERIFY, 0);
		if (r != SC_SUCCESS)
			break;
		r = sc_transmit_apdu(card, &apdu);
		if (r != SC_SUCCESS)
			sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "APDU transmit failed");
		break;
	case SC_PIN_CMD_CHANGE:
		if (pdata->pin_type != SC_AC_CHV)
			return SC_ERROR_INVALID_ARGUMENTS;
		if (pdata->pin2.len < 4 || pdata->pin2.len > 16) {
			sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "invalid PIN2 length");
			return SC_ERROR_INVALID_PIN_LENGTH; 
		}
		 
		r = asepcos_build_pin_apdu(card, &apdu, pdata, sbuf, sizeof(sbuf), SC_PIN_CMD_VERIFY, 0);
		if (r != SC_SUCCESS)
			break;
		r = sc_transmit_apdu(card, &apdu);
		if (r != SC_SUCCESS) {
			sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "APDU transmit failed");
			break;
		}
		if (apdu.sw1 != 0x90 || apdu.sw2 != 0x00) {
			 
			break;
		}
		 
		r = asepcos_build_pin_apdu(card, &apdu, pdata, sbuf, sizeof(sbuf), SC_PIN_CMD_CHANGE, 0);
		if (r != SC_SUCCESS)
			break;
		r = sc_transmit_apdu(card, &apdu);
		if (r != SC_SUCCESS)
			sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "APDU transmit failed");
		break;
	case SC_PIN_CMD_UNBLOCK:
		if (pdata->pin_type != SC_AC_CHV)
			return SC_ERROR_INVALID_ARGUMENTS;
		if (pdata->pin2.len < 4 || pdata->pin2.len > 16) {
			sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "invalid PIN2 length");
			return SC_ERROR_INVALID_PIN_LENGTH; 
		}
		 
		r = asepcos_build_pin_apdu(card, &apdu, pdata, sbuf, sizeof(sbuf), SC_PIN_CMD_VERIFY, 1);
		if (r != SC_SUCCESS)
			break;
		r = sc_transmit_apdu(card, &apdu);
		if (r != SC_SUCCESS) {
			sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "APDU transmit failed");
			break;
		}
		 
		r = asepcos_build_pin_apdu(card, &apdu, pdata, sbuf, sizeof(sbuf), SC_PIN_CMD_UNBLOCK, 0);
		if (r != SC_SUCCESS)
			break;
		r = sc_transmit_apdu(card, &apdu);
		if (r != SC_SUCCESS) {
			sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "APDU transmit failed");
			break;
		}
		break;
	default:
		sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "error: unknown cmd type");
		return SC_ERROR_INTERNAL;
	}
	 
	sc_mem_clear(sbuf, sizeof(sbuf));
	 
	if (r == SC_SUCCESS) {
		if (apdu.sw1 == 0x63) {
			if ((apdu.sw2 & 0xF0) == 0xC0 && tries_left != NULL)
				*tries_left = apdu.sw2 & 0x0F;
			r = SC_ERROR_PIN_CODE_INCORRECT;
			return r;
		}
		r = sc_check_sw(card, apdu.sw1, apdu.sw2);
	}

	return r;
}
