ksba_ocsp_get_digest_algo (ksba_ocsp_t ocsp)
{
  return ocsp? ocsp->digest_oid : NULL;
}
