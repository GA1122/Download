static int tls1_generate_key_block(SSL *s, unsigned char *km,
	     unsigned char *tmp, int num)
	{
	int ret;
	ret = tls1_PRF(ssl_get_algorithm2(s),
		 TLS_MD_KEY_EXPANSION_CONST,TLS_MD_KEY_EXPANSION_CONST_SIZE,
		 s->s3->server_random,SSL3_RANDOM_SIZE,
		 s->s3->client_random,SSL3_RANDOM_SIZE,
		 NULL,0,NULL,0,
		 s->session->master_key,s->session->master_key_length,
		 km,tmp,num);
#ifdef KSSL_DEBUG
	printf("tls1_generate_key_block() ==> %d byte master_key =\n\t",
                s->session->master_key_length);
	{
        int i;
        for (i=0; i < s->session->master_key_length; i++)
                {
                printf("%02X", s->session->master_key[i]);
                }
        printf("\n");  }
#endif     
	return ret;
	}
