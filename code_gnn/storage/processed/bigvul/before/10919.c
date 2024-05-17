parse_response (ksba_ocsp_t ocsp, const unsigned char *msg, size_t msglen)
{
  gpg_error_t err;
  struct tag_info ti;
  const unsigned char *msgstart;
  const unsigned char *endptr;
  const char *s;
  size_t len;


  msgstart = msg;
  err = parse_response_status (ocsp, &msg, &msglen, &len);
  if (err)
    return err;
  msglen = len;  
  if (ocsp->response_status)
    {
 
 
      return 0;
    }

   
  err = parse_sequence (&msg, &msglen, &ti);
  if (err)
    return err;
  endptr = msg + ti.length;

  ocsp->hash_offset = msg - msgstart;
  err = parse_response_data (ocsp, &msg, &msglen);
  if (err)
    return err;
  ocsp->hash_length = msg - msgstart - ocsp->hash_offset;

   
  s = msg;
  len = msglen;
  err = parse_sequence (&msg, &msglen, &ti);
  if (err)
    return err;
  parse_skip (&msg, &msglen, &ti);
  err= _ksba_ber_parse_tl (&msg, &msglen, &ti);
  if (err)
    return err;
  if (!(ti.class == CLASS_UNIVERSAL && ti.tag == TYPE_BIT_STRING
        && !ti.is_constructed) )
    err = gpg_error (GPG_ERR_INV_OBJ);
  else if (!ti.length)
    err = gpg_error (GPG_ERR_TOO_SHORT);
  else if (ti.length > msglen)
    err = gpg_error (GPG_ERR_BAD_BER);
  parse_skip (&msg, &msglen, &ti);
  len = len - msglen;
  xfree (ocsp->sigval); ocsp->sigval = NULL;
  err =  _ksba_sigval_to_sexp (s, len, &ocsp->sigval);
  if (err)
    return err;

   
  if (msg >= endptr)
    return 0;  
  err = parse_context_tag (&msg, &msglen, &ti, 0);
  if (gpg_err_code (err) == GPG_ERR_INV_OBJ)
    return 0;  
  if (err)
    return err;
  err = parse_sequence (&msg, &msglen, &ti);
  if (err)
    return err;
  if (ti.ndef)
    return gpg_error (GPG_ERR_UNSUPPORTED_ENCODING);

  {
    ksba_cert_t cert;
    struct ocsp_certlist_s *cl, **cl_tail;

    assert (!ocsp->received_certs);
    cl_tail = &ocsp->received_certs;
    endptr = msg + ti.length;
    while (msg < endptr)
      {
         
        s = msg;
        err = parse_sequence (&msg, &msglen, &ti);
        if (err)
          return err;
        err = ksba_cert_new (&cert);
        if (err)
          return err;
        err = ksba_cert_init_from_mem (cert, msg - ti.nhdr,
                                       ti.nhdr + ti.length);
        if (err)
          {
            ksba_cert_release (cert);
            return err;
          }
        parse_skip (&msg, &msglen, &ti);
        cl = xtrycalloc (1, sizeof *cl);
        if (!cl)
          err = gpg_error_from_errno (errno);
        if (err)
          {
            ksba_cert_release (cert);
            return gpg_error (GPG_ERR_ENOMEM);
          }
        cl->cert = cert;

        *cl_tail = cl;
        cl_tail = &cl->next;
      }
  }

  return 0;
}
