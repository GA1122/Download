LoadCredentialList () {
  CredentialWrapper * pCred;

  if (!credentials.IsEmpty()) {
    credentials.Rewind();
    while (credentials.Next(pCred)) {
      credentials.DeleteCurrent();
      delete pCred;
    }
  }

  credentials.Rewind();
  
  classad::ClassAdXMLParser parser;
  char buff[50000];
  
  priv_state priv = set_root_priv();

  FILE * fp = safe_fopen_wrapper(cred_index_file, "r");
  
  if (!fp) {
    dprintf (D_FULLDEBUG, "Credential database %s does not exist!\n", cred_index_file);
    set_priv (priv);
    return TRUE;
  }

  while (fgets(buff, 50000, fp)) {
    if ((buff[0] == '\n') || (buff[0] == '\r')) {
      continue;
    }
    
	classad::ClassAd * classad = parser.ParseClassAd (buff);
    int type=0;

    if ((!classad) || (!classad->EvaluateAttrInt ("Type", type))) {
      dprintf (D_ALWAYS, "Invalid classad %s\n", buff);
      set_priv (priv);
      fclose (fp);
      return FALSE;
    }

    if (type == X509_CREDENTIAL_TYPE) {
      pCred = new X509CredentialWrapper (*classad);
      credentials.Append (pCred);
    }
    else {
      dprintf (D_ALWAYS, "Invalid type %d\n",type); 
    }
  }
  fclose (fp);
  set_priv (priv);

  return TRUE;
}
