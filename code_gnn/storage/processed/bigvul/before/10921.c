parse_response_extensions (ksba_ocsp_t ocsp,
                          const unsigned char *data, size_t datalen)
{
  gpg_error_t err;
  struct tag_info ti;
  size_t length;
  char *oid = NULL;

  assert (!ocsp->response_extensions);
  err = parse_sequence (&data, &datalen, &ti);
  if (err)
    goto leave;
  length = ti.length;
  while (length)
    {
      struct ocsp_extension_s *ex;
      int is_crit;

      err = parse_sequence (&data, &datalen, &ti);
      if (err)
        goto leave;
      if (length < ti.nhdr + ti.length)
        {
          err = gpg_error (GPG_ERR_BAD_BER);
          goto leave;
        }
      length -= ti.nhdr + ti.length;

      xfree (oid);
      err = parse_object_id_into_str (&data, &datalen, &oid);
      if (err)
        goto leave;
      is_crit = 0;
      err = parse_optional_boolean (&data, &datalen, &is_crit);
      if (err)
        goto leave;
      err = parse_octet_string (&data, &datalen, &ti);
      if (err)
        goto leave;
      if (!strcmp (oid, oidstr_ocsp_nonce))
        {
          err = parse_integer (&data, &datalen, &ti);
          if (err)
            goto leave;
          if (ocsp->noncelen != ti.length
              || memcmp (ocsp->nonce, data, ti.length))
            ocsp->bad_nonce = 1;
          else
            ocsp->good_nonce = 1;
        }
      ex = xtrymalloc (sizeof *ex + strlen (oid) + ti.length);
      if (!ex)
        {
          err = gpg_error_from_errno (errno);
          goto leave;
        }
      ex->crit = is_crit;
      strcpy (ex->data, oid);
      ex->data[strlen (oid)] = 0;
      ex->off = strlen (oid) + 1;
      ex->len = ti.length;
      memcpy (ex->data + ex->off, data, ti.length);
      ex->next = ocsp->response_extensions;
      ocsp->response_extensions = ex;

      parse_skip (&data, &datalen, &ti);  
    }

 leave:
  xfree (oid);
  return err;
}
