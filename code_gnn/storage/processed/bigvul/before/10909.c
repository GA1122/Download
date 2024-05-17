ksba_ocsp_set_digest_algo (ksba_ocsp_t ocsp, const char *oid)
{
  if (!ocsp || !oid || !*oid)
    return gpg_error (GPG_ERR_INV_VALUE);
  if (ocsp->digest_oid)
    xfree (ocsp->digest_oid);
  ocsp->digest_oid = xtrystrdup (oid);
  if (!ocsp->digest_oid)
    return gpg_error_from_errno (errno);
  return 0;
}
