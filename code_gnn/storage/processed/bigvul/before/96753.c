_gcry_aes_cbc_dec (void *context, unsigned char *iv,
                   void *outbuf_arg, const void *inbuf_arg,
                   size_t nblocks)
{
  RIJNDAEL_context *ctx = context;
  unsigned char *outbuf = outbuf_arg;
  const unsigned char *inbuf = inbuf_arg;
  unsigned int burn_depth = 0;

  if (0)
    ;
#ifdef USE_AESNI
  else if (ctx->use_aesni)
    {
      _gcry_aes_aesni_cbc_dec (ctx, iv, outbuf, inbuf, nblocks);
      return;
    }
#endif  
#ifdef USE_SSSE3
  else if (ctx->use_ssse3)
    {
      _gcry_aes_ssse3_cbc_dec (ctx, iv, outbuf, inbuf, nblocks);
      return;
    }
#endif  
#ifdef USE_ARM_CE
  else if (ctx->use_arm_ce)
    {
      _gcry_aes_armv8_ce_cbc_dec (ctx, iv, outbuf, inbuf, nblocks);
      return;
    }
#endif  
  else
    {
      unsigned char savebuf[BLOCKSIZE] ATTR_ALIGNED_16;
      rijndael_cryptfn_t decrypt_fn = ctx->decrypt_fn;

      check_decryption_preparation (ctx);

      if (ctx->prefetch_dec_fn)
        ctx->prefetch_dec_fn();

      for ( ;nblocks; nblocks-- )
        {
           

          burn_depth = decrypt_fn (ctx, savebuf, inbuf);

          cipher_block_xor_n_copy_2(outbuf, savebuf, iv, inbuf, BLOCKSIZE);
          inbuf += BLOCKSIZE;
          outbuf += BLOCKSIZE;
        }

      wipememory(savebuf, sizeof(savebuf));
    }

  if (burn_depth)
    _gcry_burn_stack (burn_depth + 4 * sizeof(void *));
}