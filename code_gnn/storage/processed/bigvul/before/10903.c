ksba_ocsp_hash_request (ksba_ocsp_t ocsp,
                        void (*hasher)(void *, const void *,
                                       size_t length),
                        void *hasher_arg)
{
  (void)ocsp;
  (void)hasher;
  (void)hasher_arg;
  return gpg_error (GPG_ERR_NOT_IMPLEMENTED);
}
