release_ocsp_extensions (struct ocsp_extension_s *ex)
{
  while (ex)
    {
      struct ocsp_extension_s *tmp = ex->next;
      xfree (ex);
      ex = tmp;
    }
}
