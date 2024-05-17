parse_response_data (ksba_ocsp_t ocsp,
                     unsigned char const **data, size_t *datalen)
{
  gpg_error_t err;
  struct tag_info ti;
  const unsigned char *savedata;
  size_t savedatalen;
  size_t responses_length;

   
  err = parse_sequence (data, datalen, &ti);
  if (err)
    return err;

   
  savedata = *data;
  savedatalen = *datalen;
  err = parse_context_tag (data, datalen, &ti, 0);
  if (err)
    {
      *data = savedata;
      *datalen = savedatalen;
    }
  else
    {
       
      parse_skip (data, datalen, &ti);
    }

   
  assert (!ocsp->responder_id.name);
  assert (!ocsp->responder_id.keyid);
  err = _ksba_ber_parse_tl (data, datalen, &ti);
  if (err)
    return err;
  if (ti.length > *datalen)
    return gpg_error (GPG_ERR_BAD_BER);
  else if (ti.class == CLASS_CONTEXT && ti.tag == 1  && ti.is_constructed)
    {  
      err = _ksba_derdn_to_str (*data, ti.length, &ocsp->responder_id.name);
      if (err)
        return err;
      parse_skip (data, datalen, &ti);
    }
  else if (ti.class == CLASS_CONTEXT && ti.tag == 2  && ti.is_constructed)
    {  
      err = parse_octet_string (data, datalen, &ti);
      if (err)
        return err;
      if (!ti.length)
        return gpg_error (GPG_ERR_INV_OBJ);  
      ocsp->responder_id.keyid = xtrymalloc (ti.length);
      if (!ocsp->responder_id.keyid)
        return gpg_error_from_errno (errno);
      memcpy (ocsp->responder_id.keyid, *data, ti.length);
      ocsp->responder_id.keyidlen = ti.length;
      parse_skip (data, datalen, &ti);
    }
  else
    err = gpg_error (GPG_ERR_INV_OBJ);

   
  err = parse_asntime_into_isotime (data, datalen, ocsp->produced_at);
  if (err)
    return err;

   
  err = parse_sequence (data, datalen, &ti);
  if (err )
    return err;
  responses_length = ti.length;
  while (responses_length)
    {
      savedatalen = *datalen;
      err = parse_single_response (ocsp, data, datalen);
      if (err)
        return err;
      assert (responses_length >= savedatalen - *datalen);
      responses_length -= savedatalen - *datalen;
    }

   
  savedata = *data;
  savedatalen = *datalen;
  err = parse_context_tag (data, datalen, &ti, 1);
  if (!err)
    {
      err = parse_response_extensions (ocsp, *data, ti.length);
      if (err)
        return err;
      parse_skip (data, datalen, &ti);
    }
  else if (gpg_err_code (err) == GPG_ERR_INV_OBJ)
    {
      *data = savedata;
      *datalen = savedatalen;
    }
  else
    return err;

  return 0;
}
