_gcry_cipher_gcm_get_tag (gcry_cipher_hd_t c, unsigned char *outtag,
                          size_t taglen)
{
   
  if (c->u_mode.gcm.disallow_encryption_because_of_setiv_in_fips_mode)
    return GPG_ERR_INV_STATE;

  return _gcry_cipher_gcm_tag (c, outtag, taglen, 0);
}