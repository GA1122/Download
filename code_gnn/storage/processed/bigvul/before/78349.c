static int entersafe_preinstall_keys(sc_card_t *card,int (*install_rsa)(sc_card_t *,u8))
{
	 int r;
	 u8 sbuf[SC_MAX_APDU_BUFFER_SIZE];
	 sc_apdu_t apdu;

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);

	 { 
		  u8 rsa_index;
		  for(rsa_index=ENTERSAFE_MIN_KEY_ID;
			  rsa_index<=ENTERSAFE_MAX_KEY_ID;
			  ++rsa_index)
		  {
			   r=install_rsa(card,rsa_index);
			   SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "Preinstall rsa key failed");
		  }
	 }

	 { 
		   
		  sbuf[0] = 0;	 
		  sbuf[1] = sizeof(key_maintain);	 
		  sbuf[2] = 0x03;	 
		  sbuf[3] = ENTERSAFE_AC_ALWAYS;	 
		  sbuf[4] = ENTERSAFE_AC_ALWAYS;	 
		  sbuf[5] = ENTERSAFE_AC_NEVER;	 
		  sbuf[6] = 0x01;	 
		  sbuf[7] = 0x00;	 
		  sbuf[8] = 0x00;	 
		  memcpy(&sbuf[9], key_maintain, sizeof(key_maintain));

		  sc_format_apdu(card,&apdu,SC_APDU_CASE_3_SHORT,0xF0,0x00,0x00);
		  apdu.cla=0x84;
		  apdu.data=sbuf;
		  apdu.lc=apdu.datalen=0x19;

		  r = entersafe_transmit_apdu(card,&apdu,init_key,sizeof(init_key),0,1);
		  SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "Preinstall key maintain failed");
	 }

	 { 
		  memset(sbuf,0,sizeof(sbuf));
		  sbuf[0] = 0;	 
		  sbuf[1] = 16;	 
		  sbuf[2] = 0x0B;	 
		  sbuf[3] = ENTERSAFE_AC_ALWAYS;	 
		  sbuf[4] = 0X04;	 
		  sbuf[5] = 0x38;	 
		  sbuf[6] = 0x01;	 
		  sbuf[7] = 0xFF;	 
		  sbuf[8] = 0x00;	 

		  sc_format_apdu(card,&apdu,SC_APDU_CASE_3_SHORT,0xF0,0x00,ENTERSAFE_USER_PIN_ID);
		  apdu.cla=0x84;
		  apdu.data=sbuf;
		  apdu.lc=apdu.datalen=0x19;

		  r = entersafe_transmit_apdu(card,&apdu,init_key,sizeof(init_key),0,1);
		  SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "Preinstall user PIN failed");
	 }

	 { 
		  memset(sbuf,0,sizeof(sbuf));
		  sbuf[0] = 0;	 
		  sbuf[1] = 16;	 
		  sbuf[2] = 0x0B;	 
		  sbuf[3] = ENTERSAFE_AC_ALWAYS;	 
		  sbuf[4] = 0X08;	 
		  sbuf[5] = 0xC0;	 
		  sbuf[6] = 0x01;	 
		  sbuf[7] = 0xFF;	 
		  sbuf[8] = 0x00;	 

		  sc_format_apdu(card,&apdu,SC_APDU_CASE_3_SHORT,0xF0,0x00,ENTERSAFE_USER_PIN_ID+1);
		  apdu.cla=0x84;
		  apdu.data=sbuf;
		  apdu.lc=apdu.datalen=0x19;

		  r = entersafe_transmit_apdu(card,&apdu,init_key,sizeof(init_key),0,1);
		  SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "Preinstall user PUK failed");
	 }


	 SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_VERBOSE,SC_SUCCESS);
}
