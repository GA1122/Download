static int entersafe_pin_cmd(sc_card_t *card, struct sc_pin_cmd_data *data,
			   int *tries_left)
{
	 int r;
	 SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);
	 entersafe_init_pin_info(&data->pin1,0);
	 entersafe_init_pin_info(&data->pin2,1);
	 data->flags |= SC_PIN_CMD_NEED_PADDING;

	 if(data->cmd!=SC_PIN_CMD_UNBLOCK)
	 {
		  r = iso_ops->pin_cmd(card,data,tries_left);
		  sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "Verify rv:%i", r);
	 }
	 else
	 {
		  { 
			   sc_apdu_t apdu;
			   u8 sbuf[0x10]={0};

			   memcpy(sbuf,data->pin1.data,data->pin1.len);
			   sc_format_apdu(card, &apdu, SC_APDU_CASE_3_SHORT,0x20,0x00,data->pin_reference+1);
			   apdu.lc = apdu.datalen = sizeof(sbuf);
			   apdu.data = sbuf;

			   r = entersafe_transmit_apdu(card, &apdu,0,0,0,0);
			   SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");
		  }

		  { 
			   sc_apdu_t apdu;
			   u8 sbuf[0x12]={0};
			   
			   sbuf[0] = 0x33;
			   sbuf[1] = 0x00;
			   memcpy(sbuf+2,data->pin2.data,data->pin2.len);
			   sc_format_apdu(card, &apdu, SC_APDU_CASE_3_SHORT,0xF4,0x0B,data->pin_reference);
			   apdu.cla = 0x84;
			   apdu.lc = apdu.datalen = sizeof(sbuf);
			   apdu.data = sbuf;

			   r = entersafe_transmit_apdu(card, &apdu,key_maintain,sizeof(key_maintain),1,1);
			   SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");
		  }
	 }
	 SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_VERBOSE, r);
}