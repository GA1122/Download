static int entersafe_cipher_apdu(sc_card_t *card, sc_apdu_t *apdu,
								 u8 *key, size_t keylen,
								 u8 *buff, size_t buffsize)
{
	 EVP_CIPHER_CTX * ctx = NULL;
	 u8 iv[8]={0};
	 int len;

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);

	 assert(card);
	 assert(apdu);
	 assert(key);
	 assert(buff);

	  
	 memset(buff,0,buffsize);
	 buff[0]=apdu->lc;
	 memcpy(buff+1,apdu->data,apdu->lc);
	 buff[apdu->lc+1]=0x80;

	 ctx = EVP_CIPHER_CTX_new();
	 if (ctx == NULL)
		 SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_NORMAL, SC_ERROR_OUT_OF_MEMORY);
	 EVP_CIPHER_CTX_set_padding(ctx,0);

	 if(keylen == 8)
		  EVP_EncryptInit_ex(ctx, EVP_des_ecb(), NULL, key, iv);
	 else if (keylen == 16) 
		  EVP_EncryptInit_ex(ctx, EVP_des_ede(), NULL, key, iv);
	 else
		  SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_NORMAL, SC_ERROR_INTERNAL);
	 
	 len = apdu->lc;
	 if(!EVP_EncryptUpdate(ctx, buff, &len, buff, buffsize)){
		  sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "entersafe encryption error.");
		  SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_NORMAL, SC_ERROR_INTERNAL);
	 }
	 apdu->lc = len;

	 EVP_CIPHER_CTX_free(ctx);

	 if(apdu->lc!=buffsize)
	 {
		  sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "entersafe build cipher apdu failed.");
		  SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_VERBOSE, SC_ERROR_INTERNAL);
	 }

	 apdu->data=buff;
	 apdu->datalen=apdu->lc;

	 SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_VERBOSE, SC_SUCCESS);
}