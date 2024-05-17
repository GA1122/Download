parse_octet_string (unsigned char const **buf, size_t *len, struct tag_info *ti)
{
  gpg_error_t err;

  err= _ksba_ber_parse_tl (buf, len, ti);
  if (err)
    ;
  else if (!(ti->class == CLASS_UNIVERSAL && ti->tag == TYPE_OCTET_STRING
             && !ti->is_constructed) )
    err = gpg_error (GPG_ERR_INV_OBJ);
  else if (!ti->length)
    err = gpg_error (GPG_ERR_TOO_SHORT);
  else if (ti->length > *len)
    err = gpg_error (GPG_ERR_BAD_BER);

  return err;
}
