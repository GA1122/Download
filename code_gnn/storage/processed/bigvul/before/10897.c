ksba_ocsp_get_cert (ksba_ocsp_t ocsp, int idx)
{
  struct ocsp_certlist_s *cl;

  if (!ocsp || idx < 0)
    return NULL;

  for (cl=ocsp->received_certs; cl && idx; cl = cl->next, idx--)
    ;
  if (!cl)
    return NULL;
  ksba_cert_ref (cl->cert);
  return cl->cert;
}
