parse_object_id_into_str (unsigned char const **buf, size_t *len, char **oid)
{
  struct tag_info ti;
  gpg_error_t err;

  *oid = NULL;
  err = _ksba_ber_parse_tl (buf, len, &ti);
  if (err)
    ;
  else if (!(ti.class == CLASS_UNIVERSAL && ti.tag == TYPE_OBJECT_ID
                && !ti.is_constructed) )
    err = gpg_error (GPG_ERR_INV_OBJ);
  else if (!ti.length)
    err = gpg_error (GPG_ERR_TOO_SHORT);
  else if (ti.length > *len)
    err = gpg_error (GPG_ERR_BAD_BER);
  else if (!(*oid = ksba_oid_to_str (*buf, ti.length)))
    err = gpg_error_from_errno (errno);
  else
    {
      *buf += ti.length;
      *len -= ti.length;
    }
  return err;
}
