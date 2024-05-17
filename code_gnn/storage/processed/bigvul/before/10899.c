ksba_ocsp_get_extension (ksba_ocsp_t ocsp, ksba_cert_t cert, int idx,
                         char const **r_oid, int *r_crit,
                         unsigned char const **r_der, size_t *r_derlen)
{
  struct ocsp_extension_s *ex;

  if (!ocsp)
    return gpg_error (GPG_ERR_INV_VALUE);
  if (!ocsp->requestlist)
    return gpg_error (GPG_ERR_MISSING_ACTION);
  if (idx < 0)
    return gpg_error (GPG_ERR_INV_INDEX);

  if (cert)
    {
       
      struct ocsp_reqitem_s *ri;

      for (ri=ocsp->requestlist; ri; ri = ri->next)
        if (ri->cert == cert)
          break;
      if (!ri)
        return gpg_error (GPG_ERR_NOT_FOUND);

      for (ex=ri->single_extensions; ex && idx; ex = ex->next, idx--)
        ;
      if (!ex)
        return gpg_error (GPG_ERR_EOF);  
    }
  else
    {
       
      for (ex=ocsp->response_extensions; ex && idx; ex = ex->next, idx--)
        ;
      if (!ex)
        return gpg_error (GPG_ERR_EOF);  
    }

  if (r_oid)
    *r_oid = ex->data;
  if (r_crit)
    *r_crit = ex->crit;
  if (r_der)
    *r_der = ex->data + ex->off;
  if (r_derlen)
    *r_derlen = ex->len;

  return 0;
}
