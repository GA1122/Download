_gcry_cipher_gcm_tag (gcry_cipher_hd_t c,
                      byte * outbuf, size_t outbuflen, int check)
{
  if (!(is_tag_length_valid (outbuflen) || outbuflen >= GCRY_GCM_BLOCK_LEN))
    return GPG_ERR_INV_LENGTH;
  if (c->u_mode.gcm.datalen_over_limits)
    return GPG_ERR_INV_LENGTH;

  if (!c->marks.tag)
    {
      u32 bitlengths[2][2];

      if (!c->u_mode.gcm.ghash_fn)
        return GPG_ERR_INV_STATE;

       
      bitlengths[0][1] = be_bswap32(c->u_mode.gcm.aadlen[0] << 3);
      bitlengths[0][0] = be_bswap32((c->u_mode.gcm.aadlen[0] >> 29) |
                                    (c->u_mode.gcm.aadlen[1] << 3));
       
      bitlengths[1][1] = be_bswap32(c->u_mode.gcm.datalen[0] << 3);
      bitlengths[1][0] = be_bswap32((c->u_mode.gcm.datalen[0] >> 29) |
                                    (c->u_mode.gcm.datalen[1] << 3));

       
      do_ghash_buf(c, c->u_mode.gcm.u_tag.tag, NULL, 0, 1);
      c->u_mode.gcm.ghash_aad_finalized = 1;
      c->u_mode.gcm.ghash_data_finalized = 1;

       
      do_ghash_buf(c, c->u_mode.gcm.u_tag.tag, (byte*)bitlengths,
                   GCRY_GCM_BLOCK_LEN, 1);
      cipher_block_xor (c->u_mode.gcm.u_tag.tag, c->u_mode.gcm.tagiv,
                        c->u_mode.gcm.u_tag.tag, GCRY_GCM_BLOCK_LEN);
      c->marks.tag = 1;

      wipememory (bitlengths, sizeof (bitlengths));
      wipememory (c->u_mode.gcm.macbuf, GCRY_GCM_BLOCK_LEN);
      wipememory (c->u_mode.gcm.tagiv, GCRY_GCM_BLOCK_LEN);
      wipememory (c->u_mode.gcm.aadlen, sizeof (c->u_mode.gcm.aadlen));
      wipememory (c->u_mode.gcm.datalen, sizeof (c->u_mode.gcm.datalen));
    }

  if (!check)
    {
      if (outbuflen > GCRY_GCM_BLOCK_LEN)
        outbuflen = GCRY_GCM_BLOCK_LEN;

       
      memcpy (outbuf, c->u_mode.gcm.u_tag.tag, outbuflen);
    }
  else
    {
       
      if (!is_tag_length_valid (outbuflen)
          || !buf_eq_const (outbuf, c->u_mode.gcm.u_tag.tag, outbuflen))
        return GPG_ERR_CHECKSUM;
    }

  return 0;
}