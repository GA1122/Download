_gcry_aes_ocb_crypt (gcry_cipher_hd_t c, void *outbuf_arg,
                     const void *inbuf_arg, size_t nblocks, int encrypt)
{
  RIJNDAEL_context *ctx = (void *)&c->context.c;
  unsigned char *outbuf = outbuf_arg;
  const unsigned char *inbuf = inbuf_arg;
  unsigned int burn_depth = 0;

  if (0)
    ;
#ifdef USE_AESNI
  else if (ctx->use_aesni)
    {
      return _gcry_aes_aesni_ocb_crypt (c, outbuf, inbuf, nblocks, encrypt);
    }
#endif  
#ifdef USE_SSSE3
  else if (ctx->use_ssse3)
    {
      return _gcry_aes_ssse3_ocb_crypt (c, outbuf, inbuf, nblocks, encrypt);
    }
#endif  
#ifdef USE_ARM_CE
  else if (ctx->use_arm_ce)
    {
      return _gcry_aes_armv8_ce_ocb_crypt (c, outbuf, inbuf, nblocks, encrypt);
    }
#endif  
  else if (encrypt)
    {
      union { unsigned char x1[16] ATTR_ALIGNED_16; u32 x32[4]; } l_tmp;
      rijndael_cryptfn_t encrypt_fn = ctx->encrypt_fn;

      if (ctx->prefetch_enc_fn)
        ctx->prefetch_enc_fn();

      for ( ;nblocks; nblocks-- )
        {
          u64 i = ++c->u_mode.ocb.data_nblocks;
          const unsigned char *l = ocb_get_l(c, i);

           
          cipher_block_xor_1 (c->u_iv.iv, l, BLOCKSIZE);
          cipher_block_cpy (l_tmp.x1, inbuf, BLOCKSIZE);
           
          cipher_block_xor_1 (c->u_ctr.ctr, l_tmp.x1, BLOCKSIZE);
           
          cipher_block_xor_1 (l_tmp.x1, c->u_iv.iv, BLOCKSIZE);
          burn_depth = encrypt_fn (ctx, l_tmp.x1, l_tmp.x1);
          cipher_block_xor_1 (l_tmp.x1, c->u_iv.iv, BLOCKSIZE);
          cipher_block_cpy (outbuf, l_tmp.x1, BLOCKSIZE);

          inbuf += BLOCKSIZE;
          outbuf += BLOCKSIZE;
        }
    }
  else
    {
      union { unsigned char x1[16] ATTR_ALIGNED_16; u32 x32[4]; } l_tmp;
      rijndael_cryptfn_t decrypt_fn = ctx->decrypt_fn;

      check_decryption_preparation (ctx);

      if (ctx->prefetch_dec_fn)
        ctx->prefetch_dec_fn();

      for ( ;nblocks; nblocks-- )
        {
          u64 i = ++c->u_mode.ocb.data_nblocks;
          const unsigned char *l = ocb_get_l(c, i);

           
          cipher_block_xor_1 (c->u_iv.iv, l, BLOCKSIZE);
          cipher_block_cpy (l_tmp.x1, inbuf, BLOCKSIZE);
           
          cipher_block_xor_1 (l_tmp.x1, c->u_iv.iv, BLOCKSIZE);
          burn_depth = decrypt_fn (ctx, l_tmp.x1, l_tmp.x1);
          cipher_block_xor_1 (l_tmp.x1, c->u_iv.iv, BLOCKSIZE);
           
          cipher_block_xor_1 (c->u_ctr.ctr, l_tmp.x1, BLOCKSIZE);
          cipher_block_cpy (outbuf, l_tmp.x1, BLOCKSIZE);

          inbuf += BLOCKSIZE;
          outbuf += BLOCKSIZE;
        }
    }

  if (burn_depth)
    _gcry_burn_stack (burn_depth + 4 * sizeof(void *));

  return 0;
}