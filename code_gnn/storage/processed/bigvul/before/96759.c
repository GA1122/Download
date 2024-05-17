_gcry_aes_xts_crypt (void *context, unsigned char *tweak,
		     void *outbuf_arg, const void *inbuf_arg,
		     size_t nblocks, int encrypt)
{
  RIJNDAEL_context *ctx = context;
  unsigned char *outbuf = outbuf_arg;
  const unsigned char *inbuf = inbuf_arg;
  unsigned int burn_depth = 0;
  rijndael_cryptfn_t crypt_fn;
  u64 tweak_lo, tweak_hi, tweak_next_lo, tweak_next_hi, tmp_lo, tmp_hi, carry;

  if (0)
    ;
#ifdef USE_AESNI
  else if (ctx->use_aesni)
    {
      _gcry_aes_aesni_xts_crypt (ctx, tweak, outbuf, inbuf, nblocks, encrypt);
      return;
    }
#endif  
#ifdef USE_ARM_CE
  else if (ctx->use_arm_ce)
    {
      _gcry_aes_armv8_ce_xts_crypt (ctx, tweak, outbuf, inbuf, nblocks, encrypt);
      return;
    }
#endif  
  else
    {
      if (encrypt)
        {
          if (ctx->prefetch_enc_fn)
            ctx->prefetch_enc_fn();

          crypt_fn = ctx->encrypt_fn;
        }
      else
        {
          check_decryption_preparation (ctx);

          if (ctx->prefetch_dec_fn)
            ctx->prefetch_dec_fn();

          crypt_fn = ctx->decrypt_fn;
        }

      tweak_next_lo = buf_get_le64 (tweak + 0);
      tweak_next_hi = buf_get_le64 (tweak + 8);

      while (nblocks)
	{
	  tweak_lo = tweak_next_lo;
	  tweak_hi = tweak_next_hi;

	   
	  tmp_lo = buf_get_le64 (inbuf + 0) ^ tweak_lo;
	  tmp_hi = buf_get_le64 (inbuf + 8) ^ tweak_hi;

	  buf_put_le64 (outbuf + 0, tmp_lo);
	  buf_put_le64 (outbuf + 8, tmp_hi);

	   
	  carry = -(tweak_next_hi >> 63) & 0x87;
	  tweak_next_hi = (tweak_next_hi << 1) + (tweak_next_lo >> 63);
	  tweak_next_lo = (tweak_next_lo << 1) ^ carry;

	  burn_depth = crypt_fn (ctx, outbuf, outbuf);

	  buf_put_le64 (outbuf + 0, buf_get_le64 (outbuf + 0) ^ tweak_lo);
	  buf_put_le64 (outbuf + 8, buf_get_le64 (outbuf + 8) ^ tweak_hi);

	  outbuf += GCRY_XTS_BLOCK_LEN;
	  inbuf += GCRY_XTS_BLOCK_LEN;
	  nblocks--;
	}

      buf_put_le64 (tweak + 0, tweak_next_lo);
      buf_put_le64 (tweak + 8, tweak_next_hi);
    }

  if (burn_depth)
    _gcry_burn_stack (burn_depth + 5 * sizeof(void *));
}