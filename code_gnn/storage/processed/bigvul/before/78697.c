static int setcos_generate_store_key(sc_card_t *card,
	struct sc_cardctl_setcos_gen_store_key_info *data)
{
	struct	sc_apdu apdu;
	u8	sbuf[SC_MAX_APDU_BUFFER_SIZE];
	int	r, len;

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);

	 
	len = 0;
	if (data->op_type == OP_TYPE_GENERATE)
		sbuf[len++] = 0x92;	 
	else
		sbuf[len++] = 0x9A;	 
	sbuf[len++] = 0x00;	
	sbuf[len++] = data->mod_len / 256;	 
	sbuf[len++] = data->mod_len % 256;

	sbuf[len++] = data->pubexp_len / 256;    
	sbuf[len++] = data->pubexp_len % 256;
	memcpy(sbuf + len, data->pubexp, (data->pubexp_len + 7) / 8);
	len += (data->pubexp_len + 7) / 8;

	if (data->op_type == OP_TYPE_STORE) {
		sbuf[len++] = data->primep_len / 256;
		sbuf[len++] = data->primep_len % 256;
		memcpy(sbuf + len, data->primep, (data->primep_len + 7) / 8);
		len += (data->primep_len + 7) / 8;
		sbuf[len++] = data->primeq_len / 256;
		sbuf[len++] = data->primeq_len % 256;
		memcpy(sbuf + len, data->primeq, (data->primeq_len + 7) / 8);
		len += (data->primeq_len + 7) / 8;		
	}

	sc_format_apdu(card, &apdu, SC_APDU_CASE_3_SHORT, 0x46, 0x00, 0x00);
	apdu.cla = 0x00;
	apdu.data = sbuf;
	apdu.datalen = len;
	apdu.lc	= len;

	r = sc_transmit_apdu(card, &apdu);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");

	r = sc_check_sw(card, apdu.sw1, apdu.sw2);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "STORE/GENERATE_KEY returned error");

	SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_NORMAL, r);
}