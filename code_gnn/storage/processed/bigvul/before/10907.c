ksba_ocsp_prepare_request (ksba_ocsp_t ocsp)
{
  gpg_error_t err;
  struct ocsp_reqitem_s *ri;
  unsigned char *p;
  const unsigned char *der;
  size_t derlen;
  ksba_writer_t w1 = NULL;
  ksba_writer_t w2 = NULL;
  ksba_writer_t w3 = NULL;
  ksba_writer_t w4, w5, w6, w7;  

  if (!ocsp)
    return gpg_error (GPG_ERR_INV_VALUE);

  xfree (ocsp->request_buffer);
  ocsp->request_buffer = NULL;
  ocsp->request_buflen = 0;

  if (!ocsp->requestlist)
    return gpg_error (GPG_ERR_MISSING_ACTION);

   
  err = ksba_writer_new (&w3);
  if (!err)
    err = ksba_writer_set_mem (w3, 2048);
  if (!err)
    err = ksba_writer_new (&w2);
  if (!err)
    err = ksba_writer_new (&w1);
  if (err)
    goto leave;


   
  for (ri=ocsp->requestlist; ri; ri = ri->next)
    {
      err = ksba_writer_set_mem (w2, 256);
      if (!err)
        err = ksba_writer_set_mem (w1, 256);
      if (err)
        goto leave;

       
      err = _ksba_der_write_algorithm_identifier (w1, oidstr_sha1, NULL, 0);
      if (err)
        goto leave;

       
      err = issuer_name_hash (ri->issuer_cert, ri->issuer_name_hash);
      if (!err)
        err = _ksba_ber_write_tl (w1, TYPE_OCTET_STRING, CLASS_UNIVERSAL, 0,20);
      if (!err)
        err = ksba_writer_write (w1, ri->issuer_name_hash, 20);
      if(err)
        goto leave;

       
      err = issuer_key_hash (ri->issuer_cert, ri->issuer_key_hash);
      if (!err)
        err = _ksba_ber_write_tl (w1, TYPE_OCTET_STRING, CLASS_UNIVERSAL, 0,20);
      if (!err)
        err = ksba_writer_write (w1, ri->issuer_key_hash, 20);
      if (err)
        goto leave;

       
      err = _ksba_cert_get_serial_ptr (ri->cert, &der, &derlen);
      if (!err)
        err = _ksba_ber_write_tl (w1, TYPE_INTEGER, CLASS_UNIVERSAL, 0, derlen);
      if (!err)
        err = ksba_writer_write (w1, der, derlen);
      if (err)
        goto leave;
      xfree (ri->serialno);
      ri->serialno = xtrymalloc (derlen);
      if (!ri->serialno)
        err = gpg_error_from_errno (errno);
      if (err)
        goto leave;
      memcpy (ri->serialno, der, derlen);
      ri->serialnolen = derlen;


       
      p = ksba_writer_snatch_mem (w1, &derlen);
      if (!p)
        {
          err = ksba_writer_error (w1);
          goto leave;
        }
      err = _ksba_ber_write_tl (w2, TYPE_SEQUENCE, CLASS_UNIVERSAL,
                                1, derlen);
      if (!err)
        err = ksba_writer_write (w2, p, derlen);
      xfree (p); p = NULL;
      if (err)
        goto leave;

       

       
      p = ksba_writer_snatch_mem (w2, &derlen);
      if (!p)
        {
          err = ksba_writer_error (w2);
          goto leave;
        }
      err = _ksba_ber_write_tl (w3, TYPE_SEQUENCE, CLASS_UNIVERSAL,
                                1, derlen);
      if (!err)
        err = ksba_writer_write (w3, p, derlen);
      xfree (p); p = NULL;
      if (err)
        goto leave;

    }  

   
  w4 = w1;
  w5 = w2;
  err = ksba_writer_set_mem (w4, 2048);
  if (!err)
    err = ksba_writer_set_mem (w5, 2048);
  if (err)
    goto leave;

   
  p = ksba_writer_snatch_mem (w3, &derlen);
  if (!p)
    {
      err = ksba_writer_error (w3);
      goto leave;
    }
  err = _ksba_ber_write_tl (w4, TYPE_SEQUENCE, CLASS_UNIVERSAL,
                            1, derlen);
  if (!err)
    err = ksba_writer_write (w4, p, derlen);
  xfree (p); p = NULL;
  if (err)
    goto leave;

   
  err = write_request_extensions (ocsp, w4);

   

   

   

   
  p = ksba_writer_snatch_mem (w4, &derlen);
  if (!p)
    {
      err = ksba_writer_error (w4);
      goto leave;
    }
  err = _ksba_ber_write_tl (w5, TYPE_SEQUENCE, CLASS_UNIVERSAL,
                            1, derlen);
  if (!err)
    err = ksba_writer_write (w5, p, derlen);
  xfree (p); p = NULL;
  if (err)
    goto leave;

   
  w6 = w3;
  w7 = w4;
  err = ksba_writer_set_mem (w6, 2048);
  if (!err)
    err = ksba_writer_set_mem (w7, 2048);
  if (err)
    goto leave;

   
  p = ksba_writer_snatch_mem (w5, &derlen);
  if (!p)
    {
      err = ksba_writer_error (w5);
      goto leave;
    }
  err = _ksba_ber_write_tl (w6, TYPE_SEQUENCE, CLASS_UNIVERSAL,
                            1, derlen);
  if (!err)
    err = ksba_writer_write (w6, p, derlen);
  xfree (p); p = NULL;
  if (err)
    goto leave;

   

   

   
 
 
 
 
 
 
 
 
 
 
 
 
 


   
  p = ksba_writer_snatch_mem (w6, &derlen);
  if (!p)
    {
      err = ksba_writer_error (w6);
      goto leave;
    }
  ocsp->request_buffer = p;
  ocsp->request_buflen = derlen;
   

 leave:
  ksba_writer_release (w3);
  ksba_writer_release (w2);
  ksba_writer_release (w1);
  return err;
}
