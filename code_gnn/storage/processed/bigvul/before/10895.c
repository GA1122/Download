ksba_ocsp_add_target (ksba_ocsp_t ocsp,
                      ksba_cert_t cert, ksba_cert_t issuer_cert)
{
  struct ocsp_reqitem_s *ri;

  if (!ocsp || !cert || !issuer_cert)
    return gpg_error (GPG_ERR_INV_VALUE);

  ri = xtrycalloc (1, sizeof *ri);
  if (!ri)
    return gpg_error_from_errno (errno);
  ksba_cert_ref (cert);
  ri->cert = cert;
  ksba_cert_ref (issuer_cert);
  ri->issuer_cert = issuer_cert;

  ri->next = ocsp->requestlist;
  ocsp->requestlist = ri;

  return 0;
}
