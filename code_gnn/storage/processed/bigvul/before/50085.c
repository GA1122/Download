static CURLcode cmp_peer_pubkey(struct ssl_connect_data *connssl,
                                const char *pinnedpubkey)
{
  CURLcode result = CURLE_SSL_PINNEDPUBKEYNOTMATCH;
  struct Curl_easy *data = connssl->data;
  CERTCertificate *cert;

  if(!pinnedpubkey)
     
    return CURLE_OK;

   
  cert = SSL_PeerCertificate(connssl->handle);
  if(cert) {
     
    SECKEYPublicKey *pubkey = CERT_ExtractPublicKey(cert);
    if(pubkey) {
       
      SECItem *cert_der = PK11_DEREncodePublicKey(pubkey);
      if(cert_der) {
         
        result = Curl_pin_peer_pubkey(data, pinnedpubkey, cert_der->data,
                                      cert_der->len);
        SECITEM_FreeItem(cert_der, PR_TRUE);
      }
      SECKEY_DestroyPublicKey(pubkey);
    }
    CERT_DestroyCertificate(cert);
  }

   
  switch(result) {
  case CURLE_OK:
    infof(data, "pinned public key verified successfully!\n");
    break;
  case CURLE_SSL_PINNEDPUBKEYNOTMATCH:
    failf(data, "failed to verify pinned public key");
    break;
  default:
     
    break;
  }

  return result;
}
