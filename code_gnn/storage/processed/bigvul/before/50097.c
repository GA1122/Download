static CURLcode nss_cache_crl(SECItem *crl_der)
{
  CERTCertDBHandle *db = CERT_GetDefaultCertDB();
  CERTSignedCrl *crl = SEC_FindCrlByDERCert(db, crl_der, 0);
  if(crl) {
     
    SEC_DestroyCrl(crl);
    SECITEM_FreeItem(crl_der, PR_TRUE);
    return CURLE_OK;
  }

   
  PR_Lock(nss_crllock);

   
  if(!Curl_llist_insert_next(nss_crl_list, nss_crl_list->tail, crl_der)) {
    SECITEM_FreeItem(crl_der, PR_TRUE);
    PR_Unlock(nss_crllock);
    return CURLE_OUT_OF_MEMORY;
  }

  if(SECSuccess != CERT_CacheCRL(db, crl_der)) {
     
    PR_Unlock(nss_crllock);
    return CURLE_SSL_CRL_BADFILE;
  }

   
  SSL_ClearSessionCache();
  PR_Unlock(nss_crllock);
  return CURLE_OK;
}
