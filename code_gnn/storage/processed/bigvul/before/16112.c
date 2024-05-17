store_cred_handler(Service *  , int  , Stream *stream) {
  void * data = NULL;
  int rtnVal = FALSE;
  int rc;
  char * temp_file_name = NULL;
  bool found_cred;
  CredentialWrapper * temp_cred = NULL;
  int data_size = -1;
  classad::ClassAd * _classad = NULL;
  classad::ClassAd classad;
  std::string classad_cstr;
  char * classad_str = NULL;
  classad::ClassAdParser parser;
  ReliSock * socket = (ReliSock*)stream;
  const char * user = NULL;

  CredentialWrapper * cred_wrapper = NULL;

  if (!socket->triedAuthentication()) { 
    CondorError errstack;
    if( ! SecMan::authenticate_sock(socket, WRITE, &errstack) ) {
      dprintf (D_ALWAYS, "Unable to authenticate, qutting\n");
      goto EXIT;
    }
  }

  user = socket->getFullyQualifiedUser();
  dprintf (D_FULLDEBUG, "Request by: %s, %s\n", socket->getOwner(), user);

  socket->decode();

  if (!socket->code (classad_str)) {
    dprintf (D_ALWAYS, "Error receiving credential metadata\n"); 
    goto EXIT;
  }

  classad_cstr = classad_str;
  free (classad_str);

  _classad = parser.ParseClassAd(classad_cstr);
  if (!_classad) {
	  dprintf (D_ALWAYS, "Error: invalid credential metadata %s\n", classad_cstr.c_str());
	  goto EXIT;
  }

  classad = *_classad;
  delete _classad;
  
 
  
  int type;
  if (!classad.EvaluateAttrInt ("Type", type)) {
    dprintf (D_ALWAYS, "Missing Type attribute in classad!\n");
    goto EXIT;
  }


  if (type == X509_CREDENTIAL_TYPE) {
	cred_wrapper = new X509CredentialWrapper (classad);
    dprintf (D_ALWAYS, "Name=%s Size=%d\n", 
			 cred_wrapper->cred->GetName(), 
			 cred_wrapper->cred->GetDataSize());

  } else {
	  dprintf (D_ALWAYS, "Unsupported credential type %d\n", type);
	  goto EXIT;
  }

  cred_wrapper->cred->SetOrigOwner (socket->getOwner());  
  cred_wrapper->cred->SetOwner (user);                    

  data_size = cred_wrapper->cred->GetDataSize();
  if (data_size > MAX_CRED_DATA_SIZE) {
	  dprintf (D_ALWAYS, "ERROR: Credential data size %d > maximum allowed (%d)\n", data_size, MAX_CRED_DATA_SIZE);
	  goto EXIT;
  }

  data = malloc (data_size);
  if (data == NULL) {
	  EXCEPT("Out of memory. Aborting.");
  }
  if (!socket->code_bytes(data,data_size)) {
    dprintf (D_ALWAYS, "Error receiving credential data\n");
    goto EXIT;
  }
  cred_wrapper->cred->SetData (data, data_size);
  

  found_cred=false;
  credentials.Rewind();
  while (credentials.Next(temp_cred)) {
	  if ((strcmp(cred_wrapper->cred->GetName(), 
				  temp_cred->cred->GetName()) == 0) && 
		  (strcmp(cred_wrapper->cred->GetOwner(), 
				  temp_cred->cred->GetOwner()) == 0)) {
		  found_cred=true;
		  break;  
	  }
  }

  if (found_cred) {
	  dprintf (D_ALWAYS, "Credential %s for owner %s already exists!\n", 
			   cred_wrapper->cred->GetName(), 
			   cred_wrapper->cred->GetOwner());
	  socket->encode();
	  int rcred=CREDD_ERROR_CREDENTIAL_ALREADY_EXISTS;
	  socket->code(rcred);
	  goto EXIT;
  }

  
  temp_file_name = dircat (cred_store_dir, "credXXXXXX");
  condor_mkstemp (temp_file_name);
  cred_wrapper->SetStorageName (temp_file_name);
  
  init_user_id_from_FQN (user);
  if (!StoreData(temp_file_name,data,data_size)) {
    socket->encode();
    int rcred = CREDD_UNABLE_TO_STORE;
    socket->code(rcred);
    goto EXIT;
  }

  ((X509CredentialWrapper*)cred_wrapper)->cred->SetRealExpirationTime (
			   x509_proxy_expiration_time(temp_file_name));

  credentials.Append (cred_wrapper);
  SaveCredentialList();

  socket->encode();
  rc = CREDD_SUCCESS;
  socket->code(rc);

  dprintf( D_ALWAYS, "Credential name %s owner %s successfully stored\n",
			 cred_wrapper->cred->GetName(), cred_wrapper->cred->GetOwner() );

  if (type == X509_CREDENTIAL_TYPE) {
	((X509Credential*)cred_wrapper->cred)->display( D_FULLDEBUG );
  }
  rtnVal = TRUE;

EXIT:
  if ( data != NULL ) {
	  free (data);
  }
  if ( temp_file_name != NULL ) {
	  delete [] temp_file_name;
  }
  if ( cred_wrapper != NULL) {
	  delete cred_wrapper;
  }
  return rtnVal;
}
