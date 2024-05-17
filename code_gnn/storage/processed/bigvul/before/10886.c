get_name (ksba_cert_t cert, int idx, int use_subject, char **result)
{
  gpg_error_t err;
  char *p;
  int i;
  const char *oid;
  struct tag_info ti;
  const unsigned char *der;
  size_t off, derlen, seqlen;

  if (!cert || !cert->initialized || !result)
    return gpg_error (GPG_ERR_INV_VALUE);
  if (idx < 0)
    return gpg_error (GPG_ERR_INV_INDEX);

  *result = NULL;
  if (!idx)
    {  
      AsnNode n;

      n = _ksba_asn_find_node (cert->root,
                               (use_subject?
                                "Certificate.tbsCertificate.subject":
                                "Certificate.tbsCertificate.issuer") );
      if (!n || !n->down)
        return gpg_error (GPG_ERR_NO_VALUE);  
      n = n->down;  
      if (n->off == -1)
        return gpg_error (GPG_ERR_NO_VALUE);

      err = _ksba_dn_to_str (cert->image, n, &p);
      if (err)
        return err;
      *result = p;
      return 0;
    }

   
  for (i=0; !(err=ksba_cert_get_extension (cert, i, &oid, NULL,
                                           &off, &derlen)); i++)
    {
      if (!strcmp (oid, (use_subject?
                         oidstr_subjectAltName:oidstr_issuerAltName)))
        break;
    }
  if (err)
      return err;  

  der = cert->image + off;


   

  err = _ksba_ber_parse_tl (&der, &derlen, &ti);
  if (err)
    return err;
  if ( !(ti.class == CLASS_UNIVERSAL && ti.tag == TYPE_SEQUENCE
         && ti.is_constructed) )
    return gpg_error (GPG_ERR_INV_CERT_OBJ);
  if (ti.ndef)
    return gpg_error (GPG_ERR_NOT_DER_ENCODED);
  seqlen = ti.length;
  if (seqlen > derlen)
    return gpg_error (GPG_ERR_BAD_BER);
  if (!seqlen)
    return gpg_error (GPG_ERR_INV_CERT_OBJ);  

  while (seqlen)
    {
      err = _ksba_ber_parse_tl (&der, &derlen, &ti);
      if (err)
        return err;
      if (ti.class != CLASS_CONTEXT)
        return gpg_error (GPG_ERR_INV_CERT_OBJ);  
      if (ti.ndef)
        return gpg_error (GPG_ERR_NOT_DER_ENCODED);
      if (seqlen < ti.nhdr)
        return gpg_error (GPG_ERR_BAD_BER);
      seqlen -= ti.nhdr;
      if (seqlen < ti.length)
        return gpg_error (GPG_ERR_BAD_BER);
      seqlen -= ti.length;
      if (derlen < ti.length)
        return gpg_error (GPG_ERR_BAD_BER);
      if (!(ti.tag == 1 || ti.tag == 2 || ti.tag == 6))
        ;  
      else if (--idx)
        ;  
      else if (ti.tag == 1)
        {  
          p = xtrymalloc (ti.length+3);
          if (!p)
            return gpg_error (GPG_ERR_ENOMEM);
          *p = '<';
          memcpy (p+1, der, ti.length);
          p[ti.length+1] = '>';
          p[ti.length+2] = 0;
          *result = p;
          return 0;
        }
      else if (ti.tag == 2 || ti.tag == 6)
        {  
          char numbuf[20], *numbufp;
          size_t numbuflen;

          numbufp = smklen (numbuf, DIM(numbuf), ti.length, &numbuflen);
          p = xtrymalloc (11 + numbuflen + ti.length + 3);
          if (!p)
            return gpg_error (GPG_ERR_ENOMEM);
          *result = p;
          p = stpcpy (p, ti.tag == 2? "(8:dns-name" : "(3:uri");
          p = stpcpy (p, numbufp);
          memcpy (p, der, ti.length);
          p += ti.length;
          *p++ = ')';
          *p = 0;
          return 0;
        }

       
      der += ti.length;
      derlen -= ti.length;
    }

  return gpg_error (GPG_ERR_EOF);
}
