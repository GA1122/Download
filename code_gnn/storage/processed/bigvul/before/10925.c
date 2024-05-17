parse_single_response (ksba_ocsp_t ocsp,
                       unsigned char const **data, size_t *datalen)
{
  gpg_error_t err;
  struct tag_info ti;
  const unsigned char *savedata;
  const unsigned char *endptr;
  size_t savedatalen;
  size_t n;
  char *oid;
  ksba_isotime_t this_update, next_update, revocation_time;
  int look_for_request;
  const unsigned char *name_hash;
  const unsigned char *key_hash;
  const unsigned char *serialno;
  size_t serialnolen;
  struct ocsp_reqitem_s *request_item = NULL;

   
  err = parse_sequence (data, datalen, &ti);
  if (err)
    return err;
  endptr = *data + ti.length;

   
  err = parse_sequence (data, datalen, &ti);
  if (err)
    return err;
  err = _ksba_parse_algorithm_identifier (*data, *datalen, &n, &oid);
  if (err)
    return err;
  assert (n <= *datalen);
  *data += n;
  *datalen -= n;
   
  look_for_request = !strcmp (oid, oidstr_sha1);
  xfree (oid);

  err = parse_octet_string (data, datalen, &ti);
  if (err)
    return err;
  name_hash = *data;
 
 
 
  if (ti.length != 20)
    look_for_request = 0;  
  parse_skip (data, datalen, &ti);

  err = parse_octet_string (data, datalen, &ti);
  if (err)
    return err;
  key_hash = *data;
 
 
 
  if (ti.length != 20)
    look_for_request = 0;  
  parse_skip (data, datalen, &ti);

  err= parse_integer (data, datalen, &ti);
  if (err)
    return err;
  serialno = *data;
  serialnolen = ti.length;
 
 
 
  parse_skip (data, datalen, &ti);

  if (look_for_request)
    {
      for (request_item = ocsp->requestlist;
           request_item; request_item = request_item->next)
        if (!memcmp (request_item->issuer_name_hash, name_hash, 20)
             && !memcmp (request_item->issuer_key_hash, key_hash, 20)
             && request_item->serialnolen == serialnolen
            && !memcmp (request_item->serialno, serialno, serialnolen))
          break;  
    }


   
  *revocation_time = 0;
  err = _ksba_ber_parse_tl (data, datalen, &ti);
  if (err)
    return err;
  if (ti.length > *datalen)
    return gpg_error (GPG_ERR_BAD_BER);
  else if (ti.class == CLASS_CONTEXT && ti.tag == 0  && !ti.is_constructed)
    {  
      if (!ti.length)
        ;  
      else if (*datalen && !**data)
        {  
          (*datalen)--;
          (*data)++;
        }
      else
        return gpg_error (GPG_ERR_INV_OBJ);

      if (request_item)
        request_item->status = KSBA_STATUS_GOOD;
    }
  else if (ti.class == CLASS_CONTEXT && ti.tag == 1  && ti.is_constructed)
    {  
      ksba_crl_reason_t reason = KSBA_CRLREASON_UNSPECIFIED;

      err = parse_asntime_into_isotime (data, datalen, revocation_time);
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
          err = parse_enumerated (data, datalen, &ti, 1);
          if (err)
            return err;
          switch (**data)
            {
            case  0: reason = KSBA_CRLREASON_UNSPECIFIED; break;
            case  1: reason = KSBA_CRLREASON_KEY_COMPROMISE; break;
            case  2: reason = KSBA_CRLREASON_CA_COMPROMISE; break;
            case  3: reason = KSBA_CRLREASON_AFFILIATION_CHANGED; break;
            case  4: reason = KSBA_CRLREASON_SUPERSEDED; break;
            case  5: reason = KSBA_CRLREASON_CESSATION_OF_OPERATION; break;
            case  6: reason = KSBA_CRLREASON_CERTIFICATE_HOLD; break;
            case  8: reason = KSBA_CRLREASON_REMOVE_FROM_CRL; break;
            case  9: reason = KSBA_CRLREASON_PRIVILEGE_WITHDRAWN; break;
            case 10: reason = KSBA_CRLREASON_AA_COMPROMISE; break;
            default: reason = KSBA_CRLREASON_OTHER; break;
            }
          parse_skip (data, datalen, &ti);
        }
 
      if (request_item)
        {
          request_item->status = KSBA_STATUS_REVOKED;
          _ksba_copy_time (request_item->revocation_time, revocation_time);
          request_item->revocation_reason = reason;
        }
    }
  else if (ti.class == CLASS_CONTEXT && ti.tag == 2 && !ti.is_constructed
           && *datalen)
    {  
      if (!ti.length)
        ;  
      else if (!**data)
        {  
          (*datalen)--;
          (*data)++;
        }
      else  
        {
          err = parse_enumerated (data, datalen, &ti, 0);
          if (err)
            return err;
          fprintf (stderr, "libksba: unknownReason with an enum of "
                   "length %u detected\n",
                   (unsigned int)ti.length);
          parse_skip (data, datalen, &ti);
        }
      if (request_item)
        request_item->status = KSBA_STATUS_UNKNOWN;
    }
  else
    err = gpg_error (GPG_ERR_INV_OBJ);

   
  err = parse_asntime_into_isotime (data, datalen, this_update);
  if (err)
    return err;
 
  if (request_item)
      _ksba_copy_time (request_item->this_update, this_update);

   
  if (*data >= endptr)
    return 0;
  *next_update = 0;
  err = _ksba_ber_parse_tl (data, datalen, &ti);
  if (err)
    return err;
  if (ti.length > *datalen)
    return gpg_error (GPG_ERR_BAD_BER);
  else if (ti.class == CLASS_CONTEXT && ti.tag == 0  && ti.is_constructed)
    {  
      err = parse_asntime_into_isotime (data, datalen, next_update);
      if (err)
        return err;
 
      if (request_item)
        _ksba_copy_time (request_item->next_update, next_update);
    }
  else if (ti.class == CLASS_CONTEXT && ti.tag == 1  && ti.is_constructed)
    {  
      *data -= ti.nhdr;
      *datalen += ti.nhdr;
    }
  else
    err = gpg_error (GPG_ERR_INV_OBJ);

   
  if (*data >= endptr)
    return 0;
  err = _ksba_ber_parse_tl (data, datalen, &ti);
  if (err)
    return err;
  if (ti.length > *datalen)
    return gpg_error (GPG_ERR_BAD_BER);
  if (ti.class == CLASS_CONTEXT && ti.tag == 1  && ti.is_constructed)
    {
      if (request_item)
        {
          err = parse_single_extensions (request_item, *data, ti.length);
          if (err)
            return err;
        }
      parse_skip (data, datalen, &ti);
    }
  else
    err = gpg_error (GPG_ERR_INV_OBJ);

  return 0;
}
