release_ocsp_certlist (struct ocsp_certlist_s *cl)
{
  while (cl)
    {
      struct ocsp_certlist_s *tmp = cl->next;
      ksba_cert_release (cl->cert);
      xfree (cl);
      cl = tmp;
    }
}
