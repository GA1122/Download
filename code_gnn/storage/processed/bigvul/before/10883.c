_ksba_cert_get_serial_ptr (ksba_cert_t cert,
                           unsigned char const **ptr, size_t *length)
{
  asn_node_t n;

  if (!cert || !cert->initialized || !ptr || !length)
    return gpg_error (GPG_ERR_INV_VALUE);
  n = _ksba_asn_find_node (cert->root,
                           "Certificate.tbsCertificate.serialNumber");
  if (!n || n->off == -1)
    return gpg_error (GPG_ERR_NO_VALUE);

  *ptr = cert->image + n->off + n->nhdr;
  *length = n->len;
  return 0;
}
