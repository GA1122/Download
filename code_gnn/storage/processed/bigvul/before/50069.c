void Curl_nss_cleanup(void)
{
   
  PR_Lock(nss_initlock);
  if(initialized) {
     
    SSL_ClearSessionCache();

    if(mod && SECSuccess == SECMOD_UnloadUserModule(mod)) {
      SECMOD_DestroyModule(mod);
      mod = NULL;
    }
    NSS_ShutdownContext(nss_context);
    nss_context = NULL;
  }

   
  Curl_llist_destroy(nss_crl_list, NULL);
  nss_crl_list = NULL;

  PR_Unlock(nss_initlock);

  PR_DestroyLock(nss_initlock);
  PR_DestroyLock(nss_crllock);
  nss_initlock = NULL;

  initialized = 0;
}
