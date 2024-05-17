_gcry_cipher_gcm_initiv (gcry_cipher_hd_t c, const byte *iv, size_t ivlen)
{
  memset (c->u_mode.gcm.aadlen, 0, sizeof(c->u_mode.gcm.aadlen));
  memset (c->u_mode.gcm.datalen, 0, sizeof(c->u_mode.gcm.datalen));
  memset (c->u_mode.gcm.u_tag.tag, 0, GCRY_GCM_BLOCK_LEN);
  c->u_mode.gcm.datalen_over_limits = 0;
  c->u_mode.gcm.ghash_data_finalized = 0;
  c->u_mode.gcm.ghash_aad_finalized = 0;

  if (ivlen == 0)
    return GPG_ERR_INV_LENGTH;

  if (ivlen != GCRY_GCM_BLOCK_LEN - 4)
    {
      u32 iv_bytes[2] = {0, 0};
      u32 bitlengths[2][2];

      if (!c->u_mode.gcm.ghash_fn)
        return GPG_ERR_INV_STATE;

      memset(c->u_ctr.ctr, 0, GCRY_GCM_BLOCK_LEN);

      gcm_bytecounter_add(iv_bytes, ivlen);
      if (!gcm_check_aadlen_or_ivlen(iv_bytes))
        {
          c->u_mode.gcm.datalen_over_limits = 1;
          return GPG_ERR_INV_LENGTH;
        }

      do_ghash_buf(c, c->u_ctr.ctr, iv, ivlen, 1);

       
      bitlengths[1][1] = be_bswap32(iv_bytes[0] << 3);
      bitlengths[1][0] = be_bswap32((iv_bytes[0] >> 29) |
                                    (iv_bytes[1] << 3));
       
      bitlengths[0][1] = 0;
      bitlengths[0][0] = 0;

      do_ghash_buf(c, c->u_ctr.ctr, (byte*)bitlengths, GCRY_GCM_BLOCK_LEN, 1);

      wipememory (iv_bytes, sizeof iv_bytes);
      wipememory (bitlengths, sizeof bitlengths);
    }
  else
    {
       
      memcpy (c->u_ctr.ctr, iv, ivlen);
      c->u_ctr.ctr[12] = c->u_ctr.ctr[13] = c->u_ctr.ctr[14] = 0;
      c->u_ctr.ctr[15] = 1;
    }

  c->spec->encrypt (&c->context.c, c->u_mode.gcm.tagiv, c->u_ctr.ctr);

  gcm_add32_be128 (c->u_ctr.ctr, 1);

  c->unused = 0;
  c->marks.iv = 1;
  c->marks.tag = 0;

  return 0;
}