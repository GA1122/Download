do_setkey (RIJNDAEL_context *ctx, const byte *key, const unsigned keylen,
           gcry_cipher_hd_t hd)
{
  static int initialized = 0;
  static const char *selftest_failed = 0;
  int rounds;
  int i,j, r, t, rconpointer = 0;
  int KC;
#if defined(USE_AESNI) || defined(USE_PADLOCK) || defined(USE_SSSE3) \
    || defined(USE_ARM_CE)
  unsigned int hwfeatures;
#endif

  (void)hd;

   
  if (!fips_mode () && !initialized)
    {
      initialized = 1;
      selftest_failed = selftest ();
      if (selftest_failed)
        log_error ("%s\n", selftest_failed );
    }
  if (selftest_failed)
    return GPG_ERR_SELFTEST_FAILED;

  if( keylen == 128/8 )
    {
      rounds = 10;
      KC = 4;
    }
  else if ( keylen == 192/8 )
    {
      rounds = 12;
      KC = 6;
    }
  else if ( keylen == 256/8 )
    {
      rounds = 14;
      KC = 8;
    }
  else
    return GPG_ERR_INV_KEYLEN;

  ctx->rounds = rounds;

#if defined(USE_AESNI) || defined(USE_PADLOCK) || defined(USE_SSSE3) \
    || defined(USE_ARM_CE)
  hwfeatures = _gcry_get_hw_features ();
#endif

  ctx->decryption_prepared = 0;
#ifdef USE_PADLOCK
  ctx->use_padlock = 0;
#endif
#ifdef USE_AESNI
  ctx->use_aesni = 0;
#endif
#ifdef USE_SSSE3
  ctx->use_ssse3 = 0;
#endif
#ifdef USE_ARM_CE
  ctx->use_arm_ce = 0;
#endif

  if (0)
    {
      ;
    }
#ifdef USE_AESNI
  else if (hwfeatures & HWF_INTEL_AESNI)
    {
      ctx->encrypt_fn = _gcry_aes_aesni_encrypt;
      ctx->decrypt_fn = _gcry_aes_aesni_decrypt;
      ctx->prefetch_enc_fn = NULL;
      ctx->prefetch_dec_fn = NULL;
      ctx->use_aesni = 1;
      ctx->use_avx = !!(hwfeatures & HWF_INTEL_AVX);
      ctx->use_avx2 = !!(hwfeatures & HWF_INTEL_AVX2);
      if (hd)
        {
          hd->bulk.cfb_enc = _gcry_aes_aesni_cfb_enc;
          hd->bulk.cfb_dec = _gcry_aes_aesni_cfb_dec;
          hd->bulk.cbc_enc = _gcry_aes_aesni_cbc_enc;
          hd->bulk.cbc_dec = _gcry_aes_aesni_cbc_dec;
          hd->bulk.ctr_enc = _gcry_aes_aesni_ctr_enc;
          hd->bulk.ocb_crypt = _gcry_aes_aesni_ocb_crypt;
          hd->bulk.ocb_auth = _gcry_aes_aesni_ocb_auth;
          hd->bulk.xts_crypt = _gcry_aes_aesni_xts_crypt;
        }
    }
#endif
#ifdef USE_PADLOCK
  else if (hwfeatures & HWF_PADLOCK_AES && keylen == 128/8)
    {
      ctx->encrypt_fn = _gcry_aes_padlock_encrypt;
      ctx->decrypt_fn = _gcry_aes_padlock_decrypt;
      ctx->prefetch_enc_fn = NULL;
      ctx->prefetch_dec_fn = NULL;
      ctx->use_padlock = 1;
      memcpy (ctx->padlockkey, key, keylen);
    }
#endif
#ifdef USE_SSSE3
  else if (hwfeatures & HWF_INTEL_SSSE3)
    {
      ctx->encrypt_fn = _gcry_aes_ssse3_encrypt;
      ctx->decrypt_fn = _gcry_aes_ssse3_decrypt;
      ctx->prefetch_enc_fn = NULL;
      ctx->prefetch_dec_fn = NULL;
      ctx->use_ssse3 = 1;
      if (hd)
        {
          hd->bulk.cfb_enc = _gcry_aes_ssse3_cfb_enc;
          hd->bulk.cfb_dec = _gcry_aes_ssse3_cfb_dec;
          hd->bulk.cbc_enc = _gcry_aes_ssse3_cbc_enc;
          hd->bulk.cbc_dec = _gcry_aes_ssse3_cbc_dec;
          hd->bulk.ctr_enc = _gcry_aes_ssse3_ctr_enc;
          hd->bulk.ocb_crypt = _gcry_aes_ssse3_ocb_crypt;
          hd->bulk.ocb_auth = _gcry_aes_ssse3_ocb_auth;
        }
    }
#endif
#ifdef USE_ARM_CE
  else if (hwfeatures & HWF_ARM_AES)
    {
      ctx->encrypt_fn = _gcry_aes_armv8_ce_encrypt;
      ctx->decrypt_fn = _gcry_aes_armv8_ce_decrypt;
      ctx->prefetch_enc_fn = NULL;
      ctx->prefetch_dec_fn = NULL;
      ctx->use_arm_ce = 1;
      if (hd)
        {
          hd->bulk.cfb_enc = _gcry_aes_armv8_ce_cfb_enc;
          hd->bulk.cfb_dec = _gcry_aes_armv8_ce_cfb_dec;
          hd->bulk.cbc_enc = _gcry_aes_armv8_ce_cbc_enc;
          hd->bulk.cbc_dec = _gcry_aes_armv8_ce_cbc_dec;
          hd->bulk.ctr_enc = _gcry_aes_armv8_ce_ctr_enc;
          hd->bulk.ocb_crypt = _gcry_aes_armv8_ce_ocb_crypt;
          hd->bulk.ocb_auth = _gcry_aes_armv8_ce_ocb_auth;
          hd->bulk.xts_crypt = _gcry_aes_armv8_ce_xts_crypt;
        }
    }
#endif
  else
    {
      ctx->encrypt_fn = do_encrypt;
      ctx->decrypt_fn = do_decrypt;
      ctx->prefetch_enc_fn = prefetch_enc;
      ctx->prefetch_dec_fn = prefetch_dec;
    }

   

  if (0)
    {
      ;
    }
#ifdef USE_AESNI
  else if (ctx->use_aesni)
    _gcry_aes_aesni_do_setkey (ctx, key);
#endif
#ifdef USE_SSSE3
  else if (ctx->use_ssse3)
    _gcry_aes_ssse3_do_setkey (ctx, key);
#endif
#ifdef USE_ARM_CE
  else if (ctx->use_arm_ce)
    _gcry_aes_armv8_ce_setkey (ctx, key);
#endif
  else
    {
      const byte *sbox = ((const byte *)encT) + 1;
      union
        {
          PROPERLY_ALIGNED_TYPE dummy;
          byte data[MAXKC][4];
          u32 data32[MAXKC];
        } tkk[2];
#define k      tkk[0].data
#define k_u32  tkk[0].data32
#define tk     tkk[1].data
#define tk_u32 tkk[1].data32
#define W      (ctx->keyschenc)
#define W_u32  (ctx->keyschenc32)

      prefetch_enc();

      for (i = 0; i < keylen; i++)
        {
          k[i >> 2][i & 3] = key[i];
        }

      for (j = KC-1; j >= 0; j--)
        {
          tk_u32[j] = k_u32[j];
        }
      r = 0;
      t = 0;
       
      for (j = 0; (j < KC) && (r < rounds + 1); )
        {
          for (; (j < KC) && (t < 4); j++, t++)
            {
              W_u32[r][t] = le_bswap32(tk_u32[j]);
            }
          if (t == 4)
            {
              r++;
              t = 0;
            }
        }

      while (r < rounds + 1)
        {
           
          tk[0][0] ^= sbox[tk[KC-1][1] * 4];
          tk[0][1] ^= sbox[tk[KC-1][2] * 4];
          tk[0][2] ^= sbox[tk[KC-1][3] * 4];
          tk[0][3] ^= sbox[tk[KC-1][0] * 4];
          tk[0][0] ^= rcon[rconpointer++];

          if (KC != 8)
            {
              for (j = 1; j < KC; j++)
                {
                  tk_u32[j] ^= tk_u32[j-1];
                }
            }
          else
            {
              for (j = 1; j < KC/2; j++)
                {
                  tk_u32[j] ^= tk_u32[j-1];
                }
              tk[KC/2][0] ^= sbox[tk[KC/2 - 1][0] * 4];
              tk[KC/2][1] ^= sbox[tk[KC/2 - 1][1] * 4];
              tk[KC/2][2] ^= sbox[tk[KC/2 - 1][2] * 4];
              tk[KC/2][3] ^= sbox[tk[KC/2 - 1][3] * 4];
              for (j = KC/2 + 1; j < KC; j++)
                {
                  tk_u32[j] ^= tk_u32[j-1];
                }
            }

           
          for (j = 0; (j < KC) && (r < rounds + 1); )
            {
              for (; (j < KC) && (t < 4); j++, t++)
                {
                  W_u32[r][t] = le_bswap32(tk_u32[j]);
                }
              if (t == 4)
                {
                  r++;
                  t = 0;
                }
            }
        }
#undef W
#undef tk
#undef k
#undef W_u32
#undef tk_u32
#undef k_u32
      wipememory(&tkk, sizeof(tkk));
    }

  return 0;
}