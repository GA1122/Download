static int entersafe_preinstall_rsa_2048(sc_card_t *card,u8 key_id)
{
	u8 sbuf[SC_MAX_APDU_BUFFER_SIZE];
	sc_apdu_t apdu;
	int ret=0;
	static u8 const rsa_key_e[] =
	{
		'E', 0x04, 0x01, 0x00, 0x01, 0x00
	};

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);

	 
	sbuf[0] = 0x04;  
	sbuf[1] = 0x0a;  
	sbuf[2] = 0x22;	 
	sbuf[3] = 0x34;	 
	sbuf[4] = 0x04;	 
	sbuf[5] = 0x34;	 
	sbuf[6] = 0x40;	 
	sbuf[7] = 0x00;	 
	sbuf[8] = 0x00;	 
	memcpy(&sbuf[9], rsa_key_e, sizeof(rsa_key_e));
	sbuf[9 + sizeof(rsa_key_e) + 0] = 'C'+'R'+'T';
	sbuf[9 + sizeof(rsa_key_e) + 1] = 0x82;
	sbuf[9 + sizeof(rsa_key_e) + 2] = 0x04;
	sbuf[9 + sizeof(rsa_key_e) + 3] = 0x00;

	sc_format_apdu(card, &apdu, SC_APDU_CASE_3_SHORT,0xF0,0x00,key_id);
	apdu.cla=0x84;
	apdu.data=sbuf;
	apdu.lc=apdu.datalen=9 + sizeof(rsa_key_e) + 4;

	ret = entersafe_transmit_apdu(card,&apdu,init_key,sizeof(init_key),0,1);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, ret, "Preinstall rsa failed");

	 
	sbuf[0] = 0x01;	 
	sbuf[1] = 0x0A;	 
	sbuf[2] = 0x2A;	 
	sbuf[3] = ENTERSAFE_AC_ALWAYS;	 
	sbuf[4] = 0x04;	 
	sbuf[5] = ENTERSAFE_AC_ALWAYS;	 
	sbuf[6] = 0x40;	 
	sbuf[7] = 0x00;	 
	sbuf[8] = 0x00;	 
	memcpy(&sbuf[9], rsa_key_e, sizeof(rsa_key_e));		
	sbuf[9 + sizeof(rsa_key_e) + 0] = 'N';
	sbuf[9 + sizeof(rsa_key_e) + 1] = 0x82;
	sbuf[9 + sizeof(rsa_key_e) + 2] = 0x01;
	sbuf[9 + sizeof(rsa_key_e) + 3] = 0x00;

	sc_format_apdu(card,&apdu,SC_APDU_CASE_3_SHORT,0xF0,0x00,key_id);
	apdu.cla=0x84;
	apdu.data=sbuf;
	apdu.lc=apdu.datalen=9 + sizeof(rsa_key_e) + 4;

	ret=entersafe_transmit_apdu(card,&apdu,init_key,sizeof(init_key),0,1);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, ret, "Preinstall rsa failed");

	SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_VERBOSE,SC_SUCCESS);
}
