get_tls_handshake_key (const struct key_type *key_type,
		       struct key_ctx_bi *ctx,
		       const char *passphrase_file,
		       const int key_direction,
		       const unsigned int flags)
{
  if (passphrase_file && key_type->hmac_length)
    {
      struct key2 key2;
      struct key_type kt = *key_type;
      struct key_direction_state kds;

       
      kt.cipher_length = 0;
      kt.cipher = NULL;

      if (flags & GHK_INLINE)
	{
	   
	  read_key_file (&key2, passphrase_file, RKF_INLINE|RKF_MUST_SUCCEED);

	   
	  if (key2.n == 2)
	    msg (M_INFO, "Control Channel Authentication: tls-auth using INLINE static key file");
	  else
	    msg (M_FATAL, "INLINE tls-auth file lacks the requisite 2 keys");
	}
      else
      {
	 
	read_key_file (&key2, passphrase_file, 0);

	 
	if (key2.n == 2)
	  {
	    msg (M_INFO,
		 "Control Channel Authentication: using '%s' as a " PACKAGE_NAME " static key file",
		 passphrase_file);
	  }
	else
	  {
	    int hash_size;

	    CLEAR (key2);

	     
	    hash_size = read_passphrase_hash (passphrase_file,
					      kt.digest,
					      key2.keys[0].hmac,
					      MAX_HMAC_KEY_LENGTH);
	    ASSERT (hash_size == kt.hmac_length);

	     
	    key2.n = 1;

	    msg (M_INFO,
		 "Control Channel Authentication: using '%s' as a free-form passphrase file",
		 passphrase_file);
	  }
      }
       

      key_direction_state_init (&kds, key_direction);
      must_have_n_keys (passphrase_file, "tls-auth", &key2, kds.need_keys);

       

      init_key_ctx (&ctx->encrypt, &key2.keys[kds.out_key], &kt, OPENVPN_OP_ENCRYPT,
		    "Outgoing Control Channel Authentication");
      init_key_ctx (&ctx->decrypt, &key2.keys[kds.in_key], &kt, OPENVPN_OP_DECRYPT,
		    "Incoming Control Channel Authentication");

      CLEAR (key2);
    }
  else
    {
      CLEAR (*ctx);
    }
}
