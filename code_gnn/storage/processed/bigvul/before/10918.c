parse_optional_boolean (unsigned char const **buf, size_t *len, int *r_bool)
{
  gpg_error_t err;
  struct tag_info ti;

  err = _ksba_ber_parse_tl (buf, len, &ti);
  if (err)
    ;
  else if (!ti.length)
    err = gpg_error (GPG_ERR_TOO_SHORT);
  else if (ti.length > *len)
    err = gpg_error (GPG_ERR_BAD_BER);
  else if (ti.class == CLASS_UNIVERSAL && ti.tag == TYPE_BOOLEAN
           && !ti.is_constructed)
    {
      if (ti.length != 1)
        err = gpg_error (GPG_ERR_BAD_BER);
      *r_bool = !!**buf;
      parse_skip (buf, len, &ti);
    }
  else
    {  
      *buf -= ti.nhdr;
      *len += ti.nhdr;
    }

  return err;
}
