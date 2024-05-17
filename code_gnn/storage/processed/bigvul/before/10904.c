ksba_ocsp_hash_response (ksba_ocsp_t ocsp,
                         const unsigned char *msg, size_t msglen,
                         void (*hasher)(void *, const void *, size_t length),
                         void *hasher_arg)

{
  if (!ocsp || !msg || !hasher)
    return gpg_error (GPG_ERR_INV_VALUE);
  if (!ocsp->hash_length)
    return gpg_error (GPG_ERR_MISSING_ACTION);
  if (ocsp->hash_offset + ocsp->hash_length >= msglen)
    return gpg_error (GPG_ERR_CONFLICT);

  hasher (hasher_arg, msg + ocsp->hash_offset, ocsp->hash_length);
  return 0;
}
