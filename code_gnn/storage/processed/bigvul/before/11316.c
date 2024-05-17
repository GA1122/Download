mac_init (digest_hd_st * td, gnutls_mac_algorithm_t mac, opaque * secret,
          int secret_size, int ver)
{
  int ret = 0;

  if (mac == GNUTLS_MAC_NULL)
    {
      return GNUTLS_E_HASH_FAILED;
    }

  if (ver == GNUTLS_SSL3)
    {                            
      ret = _gnutls_mac_init_ssl3 (td, mac, secret, secret_size);
    }
  else
    {                            
      ret = _gnutls_hmac_init (td, mac, secret, secret_size);
    }

  return ret;
}
