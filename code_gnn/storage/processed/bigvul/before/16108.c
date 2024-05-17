SaveCredentialList() {
  priv_state priv = set_root_priv();
  FILE * fp = safe_fopen_wrapper(cred_index_file, "w");
  if (!fp) {
    set_priv (priv);
    dprintf (D_ALWAYS, "Unable to open credential index file %s!\n", cred_index_file);
    return FALSE;
  }


  classad::ClassAdXMLUnParser unparser;
  CredentialWrapper * pCred = NULL;

  credentials.Rewind();
  while (credentials.Next(pCred)) {
    const classad::ClassAd * pclassad = pCred->cred->GetMetadata();
	classad::ClassAd temp_classad(*pclassad);  
    std::string buff;
    unparser.Unparse (buff, &temp_classad);
    fprintf (fp, "%s\n", buff.c_str());
  }

  fclose (fp);
  
  set_priv (priv);
  return TRUE;
}
