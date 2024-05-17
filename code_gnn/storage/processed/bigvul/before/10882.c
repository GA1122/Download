_ksba_cert_get_public_key_ptr (ksba_cert_t cert,
                               unsigned char const **ptr, size_t *length)
{
  asn_node_t n;

  if (!cert || !cert->initialized || !ptr || !length)
    return gpg_error (GPG_ERR_INV_VALUE);

  n = _ksba_asn_find_node (cert->root,
                           "Certificate.tbsCertificate.subjectPublicKeyInfo");
  if (!n || !n->down || !n->down->right)
    return gpg_error (GPG_ERR_NO_VALUE);  
  n = n->down->right;
  if (n->off == -1)
    return gpg_error (GPG_ERR_NO_VALUE);
  *ptr = cert->image + n->off + n->nhdr;
  *length = n->len;
   
  if (*length && !**ptr)
    {
      (*length)--;
      (*ptr)++;
    }

  return 0;
}
