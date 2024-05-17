_gcry_cipher_gcm_setiv (gcry_cipher_hd_t c, const byte *iv, size_t ivlen)
{
  c->marks.iv = 0;
  c->marks.tag = 0;
  c->u_mode.gcm.disallow_encryption_because_of_setiv_in_fips_mode = 0;

  if (fips_mode ())
    {
       
      c->u_mode.gcm.disallow_encryption_because_of_setiv_in_fips_mode = 1;
    }

  return _gcry_cipher_gcm_initiv (c, iv, ivlen);
}