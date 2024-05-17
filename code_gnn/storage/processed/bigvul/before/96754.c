_gcry_aes_cbc_enc (void *context, unsigned char *iv,
                   void *outbuf_arg, const void *inbuf_arg,
                   size_t nblocks, int cbc_mac)
{
  RIJNDAEL_context *ctx = context;
  unsigned char *outbuf = outbuf_arg;
  const unsigned char *inbuf = inbuf_arg;
  unsigned char *last_iv;
  unsigned int burn_depth = 0;

  if (0)
    ;
#ifdef USE_AESNI
  else if (ctx->use_aesni)
    {
      _gcry_aes_aesni_cbc_enc (ctx, iv, outbuf, inbuf, nblocks, cbc_mac);
      return;
    }
#endif  
#ifdef USE_SSSE3
  else if (ctx->use_ssse3)
    {
      _gcry_aes_ssse3_cbc_enc (ctx, iv, outbuf, inbuf, nblocks, cbc_mac);
      return;
    }
#endif  
#ifdef USE_ARM_CE
  else if (ctx->use_arm_ce)
    {
      _gcry_aes_armv8_ce_cbc_enc (ctx, iv, outbuf, inbuf, nblocks, cbc_mac);
      return;
    }
#endif  
  else
    {
      rijndael_cryptfn_t encrypt_fn = ctx->encrypt_fn;

      if (ctx->prefetch_enc_fn)
        ctx->prefetch_enc_fn();

      last_iv = iv;

      for ( ;nblocks; nblocks-- )
        {
          cipher_block_xor(outbuf, inbuf, last_iv, BLOCKSIZE);

          burn_depth = encrypt_fn (ctx, outbuf, outbuf);

          last_iv = outbuf;
          inbuf += BLOCKSIZE;
          if (!cbc_mac)
            outbuf += BLOCKSIZE;
        }

      if (last_iv != iv)
        cipher_block_cpy (iv, last_iv, BLOCKSIZE);
    }

  if (burn_depth)
    _gcry_burn_stack (burn_depth + 4 * sizeof(void *));
}