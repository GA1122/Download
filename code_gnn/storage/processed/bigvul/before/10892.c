issuer_key_hash (ksba_cert_t cert, unsigned char *sha1_buffer)
{
  gpg_error_t err;
  const unsigned char *ptr;
  size_t length, dummy;

  err = _ksba_cert_get_public_key_ptr (cert, &ptr, &length);
  if (!err)
    {
      err = _ksba_hash_buffer (NULL, ptr, length, 20, sha1_buffer, &dummy);
      if (!err && dummy != 20)
        err = gpg_error (GPG_ERR_BUG);
    }
  return err;
}
