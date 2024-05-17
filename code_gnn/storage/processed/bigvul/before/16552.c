email_open( const char *email_addr, const char *subject )
{
	char *Mailer;
	char *SmtpServer = NULL;
	char *FromAddress = NULL;
	char *FinalSubject;
	char *FinalAddr;
	char *temp;
	int token_boundary;
	int num_addresses;
	int arg_index;
	FILE *mailerstream;

	if ( (Mailer = param("MAIL")) == NULL ) {
		dprintf(D_FULLDEBUG,
			"Trying to email, but MAIL not specified in config file\n");
		return NULL;
	}

	 
	if ( subject ) {
		size_t prolog_length = strlen(EMAIL_SUBJECT_PROLOG);
		size_t subject_length = strlen(subject);
		FinalSubject = (char *)malloc(prolog_length + subject_length + 1);
		memcpy(FinalSubject, EMAIL_SUBJECT_PROLOG, prolog_length);
		memcpy(&FinalSubject[prolog_length], subject, subject_length);
		FinalSubject[prolog_length + subject_length] = '\0';
	}
	else {
		FinalSubject = strdup(EMAIL_SUBJECT_PROLOG);
	}

	 
	FromAddress = param("MAIL_FROM");
	
#ifdef WIN32
	 
	if ( (SmtpServer=param("SMTP_SERVER")) == NULL ) {
		dprintf(D_FULLDEBUG,
			"Trying to email, but SMTP_SERVER not specified in config file\n");
		free(Mailer);
		free(FinalSubject);
		if (FromAddress) free(FromAddress);
		return NULL;
	}
#endif 	

	 
	if ( email_addr ) {
		FinalAddr = strdup(email_addr);
	} else {
		if ( (FinalAddr = param("CONDOR_ADMIN")) == NULL ) {
			dprintf(D_FULLDEBUG,
				"Trying to email, but CONDOR_ADMIN not specified in config file\n");
			free(Mailer);
			free(FinalSubject);
			if (FromAddress) free(FromAddress);
			if (SmtpServer) free(SmtpServer);
			return NULL;
		}
	}

	 
	token_boundary = TRUE;
	num_addresses = 0;
	for (temp = FinalAddr; *temp != '\0'; temp++) {
		if (*temp == ',' || *temp == ' ') {
			*temp = '\0';
			token_boundary = TRUE;
		}
		else if (token_boundary) {
			num_addresses++;
			token_boundary = FALSE;
		}
	}
	if (num_addresses == 0) {
		dprintf(D_FULLDEBUG, "Trying to email, but address list is empty\n");
		free(Mailer);
		free(FinalSubject);
		if (FromAddress) free(FromAddress);
		if (SmtpServer) free(SmtpServer);
		free(FinalAddr);
		return NULL;
	}

	 
	const char * * final_args;
	final_args = (char const * *)malloc((8 + num_addresses) * sizeof(char*));
	if (final_args == NULL) {
		EXCEPT("Out of memory");
	}
	arg_index = 0;
	final_args[arg_index++] = Mailer;
	final_args[arg_index++] = "-s";
	final_args[arg_index++] = FinalSubject;
	if (FromAddress) {
		final_args[arg_index++] = "-f";
		final_args[arg_index++] = FromAddress;
	}
	if (SmtpServer) {
		final_args[arg_index++] = "-relay";
		final_args[arg_index++] = SmtpServer;
	}
	temp = FinalAddr;
	for (;;) {
		while (*temp == '\0') temp++;
		final_args[arg_index++] = temp;
		if (--num_addresses == 0) break;
		while (*temp != '\0') temp++;
	}
	final_args[arg_index] = NULL;

 
	 
#ifdef WIN32
	mailerstream = email_open_implementation(Mailer, final_args);
#else
	mailerstream = email_open_implementation(final_args);
#endif

	if ( mailerstream ) {
		fprintf(mailerstream,"This is an automated email from the Condor "
			"system\non machine \"%s\".  Do not reply.\n\n",get_local_fqdn().Value());
	}

	 
	free(Mailer);
	free(FinalSubject);
	if (FromAddress) free(FromAddress);
	if (SmtpServer) free(SmtpServer);
	free(FinalAddr);
	free(final_args);

	return mailerstream;
}
