static int entersafe_mac_apdu(sc_card_t *card, sc_apdu_t *apdu,
							  u8 * key,size_t keylen,
							  u8 * buff,size_t buffsize)
{
	 int r;
	 u8 iv[8];
	 u8 *tmp=0,*tmp_rounded=NULL;
	 size_t tmpsize=0,tmpsize_rounded=0;
	 int outl=0;
	 EVP_CIPHER_CTX * ctx = NULL;

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);

	 assert(card);
	 assert(apdu);
	 assert(key);
	 assert(buff);

	 if(apdu->cse != SC_APDU_CASE_3_SHORT)
		  return SC_ERROR_INTERNAL;
	 if(keylen!=8 && keylen!=16)
		  return SC_ERROR_INTERNAL;

	 r=entersafe_gen_random(card,iv,sizeof(iv));
	 SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL,r,"entersafe gen random failed");

	  
	 if ((r=sc_apdu_get_octets(card->ctx, apdu, &tmp, &tmpsize,SC_PROTO_RAW)) != SC_SUCCESS)
		  goto out;

	  
	 tmpsize_rounded=(tmpsize/8+1)*8;

	 tmp_rounded = malloc(tmpsize_rounded);
	 if (tmp_rounded == NULL)
	 {
		  r =  SC_ERROR_OUT_OF_MEMORY;
		  goto out;
	 }
	 
	  
	 memset(tmp_rounded,0,tmpsize_rounded);
	 memcpy(tmp_rounded,tmp,tmpsize);
	 tmp_rounded[4]+=4;
	 tmp_rounded[tmpsize]=0x80;

	  
	 ctx = EVP_CIPHER_CTX_new();
	 if (ctx == NULL) {
		r =  SC_ERROR_OUT_OF_MEMORY;
		goto out;
	 }
	 EVP_CIPHER_CTX_set_padding(ctx,0);
	 EVP_EncryptInit_ex(ctx, EVP_des_cbc(), NULL, key, iv);

	 if(tmpsize_rounded>8){
		  if(!EVP_EncryptUpdate(ctx,tmp_rounded,&outl,tmp_rounded,tmpsize_rounded-8)){
			   r = SC_ERROR_INTERNAL;
			   goto out;			   
		  }
	 }
	  
	 if(keylen==8)
	 {
		  if(!EVP_EncryptUpdate(ctx,tmp_rounded+outl,&outl,tmp_rounded+outl,8)){
			   r = SC_ERROR_INTERNAL;
			   goto out;			   
		  }
	 }
	 else
	 {
		  EVP_EncryptInit_ex(ctx, EVP_des_ede_cbc(), NULL, key,tmp_rounded+outl-8);
		  if(!EVP_EncryptUpdate(ctx,tmp_rounded+outl,&outl,tmp_rounded+outl,8)){
			   r = SC_ERROR_INTERNAL;
			   goto out;			   
		  }
	 }

	 memcpy(buff,apdu->data,apdu->lc);
	  
	 memcpy(buff+apdu->lc,tmp_rounded+tmpsize_rounded-8,4);
	 apdu->data=buff;
	 apdu->lc+=4;
	 apdu->datalen=apdu->lc;

out:
	 if(tmp)
		  free(tmp);
	 if(tmp_rounded)
		  free(tmp_rounded);
	 if  (ctx)
		EVP_CIPHER_CTX_free(ctx);

	 SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_VERBOSE, r);
}