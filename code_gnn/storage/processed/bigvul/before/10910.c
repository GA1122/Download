ksba_ocsp_set_nonce (ksba_ocsp_t ocsp, unsigned char *nonce, size_t noncelen)
{
  if (!ocsp)
    return 0;
  if (!nonce)
    return sizeof ocsp->nonce;
  if (noncelen > sizeof ocsp->nonce)
    noncelen = sizeof ocsp->nonce;
  if (noncelen)
    {
      memcpy (ocsp->nonce, nonce, noncelen);
       
      ocsp->nonce[0] &= 0x7f;
    }
  ocsp->noncelen = noncelen;
  return noncelen;
}