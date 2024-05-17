_gcry_cipher_gcm_encrypt (gcry_cipher_hd_t c,
                          byte *outbuf, size_t outbuflen,
                          const byte *inbuf, size_t inbuflen)
{
  static const unsigned char zerobuf[MAX_BLOCKSIZE];
  gcry_err_code_t err;

  if (c->spec->blocksize != GCRY_GCM_BLOCK_LEN)
    return GPG_ERR_CIPHER_ALGO;
  if (outbuflen < inbuflen)
    return GPG_ERR_BUFFER_TOO_SHORT;
  if (c->u_mode.gcm.datalen_over_limits)
    return GPG_ERR_INV_LENGTH;
  if (c->marks.tag
      || c->u_mode.gcm.ghash_data_finalized
      || !c->u_mode.gcm.ghash_fn)
    return GPG_ERR_INV_STATE;

  if (!c->marks.iv)
    _gcry_cipher_gcm_setiv (c, zerobuf, GCRY_GCM_BLOCK_LEN);

  if (c->u_mode.gcm.disallow_encryption_because_of_setiv_in_fips_mode)
    return GPG_ERR_INV_STATE;

  if (!c->u_mode.gcm.ghash_aad_finalized)
    {
       
      do_ghash_buf(c, c->u_mode.gcm.u_tag.tag, NULL, 0, 1);
      c->u_mode.gcm.ghash_aad_finalized = 1;
    }

  gcm_bytecounter_add(c->u_mode.gcm.datalen, inbuflen);
  if (!gcm_check_datalen(c->u_mode.gcm.datalen))
    {
      c->u_mode.gcm.datalen_over_limits = 1;
      return GPG_ERR_INV_LENGTH;
    }

  while (inbuflen)
    {
      size_t currlen = inbuflen;

       
      if (currlen > 24 * 1024)
	currlen = 24 * 1024;

      err = gcm_ctr_encrypt(c, outbuf, outbuflen, inbuf, currlen);
      if (err != 0)
	return err;

      do_ghash_buf(c, c->u_mode.gcm.u_tag.tag, outbuf, currlen, 0);

      outbuf += currlen;
      inbuf += currlen;
      outbuflen -= currlen;
      inbuflen -= currlen;
    }

  return 0;
}