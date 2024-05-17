ksba_ocsp_get_responder_id (ksba_ocsp_t ocsp,
                            char **r_name, ksba_sexp_t *r_keyid)
{
  if (r_name)
    *r_name = NULL;
  if (r_keyid)
    *r_keyid = NULL;

  if (!ocsp)
    return gpg_error (GPG_ERR_INV_VALUE);

  if (ocsp->responder_id.name && r_name)
    {
      *r_name = xtrystrdup (ocsp->responder_id.name);
      if (!*r_name)
        return gpg_error_from_errno (errno);
    }
  else if (ocsp->responder_id.keyid && r_keyid)
    {
      char numbuf[50];
      size_t numbuflen;

      sprintf (numbuf,"(%lu:", (unsigned long)ocsp->responder_id.keyidlen);
      numbuflen = strlen (numbuf);
      *r_keyid = xtrymalloc (numbuflen + ocsp->responder_id.keyidlen + 2);
      if (!*r_keyid)
        return gpg_error_from_errno (errno);
      strcpy (*r_keyid, numbuf);
      memcpy (*r_keyid+numbuflen,
              ocsp->responder_id.keyid, ocsp->responder_id.keyidlen);
      (*r_keyid)[numbuflen + ocsp->responder_id.keyidlen] = ')';
      (*r_keyid)[numbuflen + ocsp->responder_id.keyidlen + 1] = 0;
    }
  else
    gpg_error (GPG_ERR_NO_DATA);

  return 0;
}
