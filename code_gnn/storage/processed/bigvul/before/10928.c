write_request_extensions (ksba_ocsp_t ocsp, ksba_writer_t wout)
{
  gpg_error_t err;
  unsigned char *buf;
  size_t buflen;
  unsigned char *p;
  size_t derlen;
  ksba_writer_t w1 = NULL;
  ksba_writer_t w2 = NULL;

  if (!ocsp->noncelen)
    return 0;  

   
  err = ksba_writer_new (&w2);
  if (!err)
    err = ksba_writer_set_mem (w2, 256);
  if (!err)
    err = ksba_writer_new (&w1);
  if (!err)
    err = ksba_writer_set_mem (w1, 256);
  if (err)
    goto leave;

   
  err = ksba_oid_from_str (oidstr_ocsp_nonce, &buf, &buflen);
  if (err)
    goto leave;
  err = _ksba_ber_write_tl (w1, TYPE_OBJECT_ID, CLASS_UNIVERSAL, 0, buflen);
  if (!err)
    err = ksba_writer_write (w1, buf, buflen);
  xfree (buf); buf = NULL;
   
  if (!err)
    err = _ksba_ber_write_tl (w1, TYPE_OCTET_STRING, CLASS_UNIVERSAL, 0,
                              2+ocsp->noncelen);
  if (!err)
    err = _ksba_ber_write_tl (w1, TYPE_INTEGER, CLASS_UNIVERSAL, 0,
                              ocsp->noncelen);
  if (!err)
    err = ksba_writer_write (w1, ocsp->nonce, ocsp->noncelen);

   
  p = ksba_writer_snatch_mem (w1, &derlen);
  if (!p)
    {
      err = ksba_writer_error (w1);
      goto leave;
    }
  err = _ksba_ber_write_tl (w2, TYPE_SEQUENCE, CLASS_UNIVERSAL, 1, derlen);
  if (!err)
    err = ksba_writer_write (w2, p, derlen);
  xfree (p); p = NULL;

   
  err = ksba_writer_set_mem (w1, 256);
  if (err)
    goto leave;
  p = ksba_writer_snatch_mem (w2, &derlen);
  if (!p)
    {
      err = ksba_writer_error (w2);
      goto leave;
    }
  err = _ksba_ber_write_tl (w1, TYPE_SEQUENCE, CLASS_UNIVERSAL, 1, derlen);
  if (!err)
    err = ksba_writer_write (w1, p, derlen);
  xfree (p); p = NULL;

   
  p = ksba_writer_snatch_mem (w1, &derlen);
  if (!p)
    {
      err = ksba_writer_error (w1);
      goto leave;
    }
  err = _ksba_ber_write_tl (wout, 2, CLASS_CONTEXT, 1, derlen);
  if (!err)
    err = ksba_writer_write (wout, p, derlen);
  xfree (p); p = NULL;


 leave:
  ksba_writer_release (w2);
  ksba_writer_release (w1);
  return err;
}
