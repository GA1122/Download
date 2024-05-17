ksba_ocsp_new (ksba_ocsp_t *r_ocsp)
{
  *r_ocsp = xtrycalloc (1, sizeof **r_ocsp);
  if (!*r_ocsp)
    return gpg_error_from_errno (errno);
  return 0;
}
