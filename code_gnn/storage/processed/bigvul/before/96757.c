_gcry_aes_ctr_enc (void *context, unsigned char *ctr,
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
      _gcry_aes_aesni_ctr_enc (ctx, ctr, outbuf, inbuf, nblocks);
      return;
    }
#endif  
#ifdef USE_SSSE3
  else if (ctx->use_ssse3)
    {
      _gcry_aes_ssse3_ctr_enc (ctx, ctr, outbuf, inbuf, nblocks);
      return;
    }
#endif  
#ifdef USE_ARM_CE
  else if (ctx->use_arm_ce)
    {
      _gcry_aes_armv8_ce_ctr_enc (ctx, ctr, outbuf, inbuf, nblocks);
      return;
    }
#endif  
  else
    {
      union { unsigned char x1[16] ATTR_ALIGNED_16; u32 x32[4]; } tmp;
      rijndael_cryptfn_t encrypt_fn = ctx->encrypt_fn;

      if (ctx->prefetch_enc_fn)
        ctx->prefetch_enc_fn();

      for ( ;nblocks; nblocks-- )
        {
           
          burn_depth = encrypt_fn (ctx, tmp.x1, ctr);
           
          cipher_block_xor(outbuf, tmp.x1, inbuf, BLOCKSIZE);
          outbuf += BLOCKSIZE;
          inbuf  += BLOCKSIZE;
           
	  cipher_block_add(ctr, 1, BLOCKSIZE);
        }

      wipememory(&tmp, sizeof(tmp));
    }

  if (burn_depth)
    _gcry_burn_stack (burn_depth + 4 * sizeof(void *));
}