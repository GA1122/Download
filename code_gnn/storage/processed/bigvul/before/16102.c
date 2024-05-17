CheckCredentials () {
  CredentialWrapper * pCred;
  credentials.Rewind();  
  dprintf (D_FULLDEBUG, "In CheckCredentials()\n");

  time_t now = time(NULL);

  while (credentials.Next(pCred)) {
    
    init_user_id_from_FQN (pCred->cred->GetOwner());
    priv_state priv = set_user_priv();

    time_t time = pCred->cred->GetRealExpirationTime();
    dprintf (D_FULLDEBUG, "Checking %s:%s = %ld\n",
	       pCred->cred->GetOwner(),
               pCred->cred->GetName(),
	       time);

    if (time - now < 0) {
      dprintf (D_FULLDEBUG, "Credential %s:%s expired!\n",
	       pCred->cred->GetOwner(),
	       pCred->cred->GetName());
    }
    else if (time - now < default_cred_expire_threshold) {
      dprintf (D_FULLDEBUG, "Credential %s:%s about to expire\n",
	       pCred->cred->GetOwner(),
	       pCred->cred->GetName());
      if (pCred->cred->GetType() == X509_CREDENTIAL_TYPE) {
	RefreshProxyThruMyProxy ((X509CredentialWrapper*)pCred);
      }
    }
    
    set_priv (priv);  
  }
}
