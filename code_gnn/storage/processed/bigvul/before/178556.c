 email_close(FILE *mailer)
 {
 	char *temp;
 	mode_t prev_umask;
 	priv_state priv;
 	char *customSig;
 
 	if ( mailer == NULL ) {
 		return;
 	}
 
 	 
 	priv = set_condor_priv();
 
         customSig = NULL;
         if ((customSig = param("EMAIL_SIGNATURE")) != NULL) {
                 fprintf( mailer, "\n\n");
               fprintf( mailer, customSig);
//                fprintf( mailer, "%s", customSig);
                 fprintf( mailer, "\n");
                 free(customSig);
         } else {
 		
 		 
 		fprintf( mailer, "\n\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n" );
 		fprintf( mailer, "Questions about this message or Condor in general?\n" );
 
 			 
 		temp = param( "CONDOR_SUPPORT_EMAIL" );
 		if( ! temp ) {
 			temp = param( "CONDOR_ADMIN" );
 		}
 		if( temp ) {
 			fprintf( mailer, "Email address of the local Condor administrator: "
 					 "%s\n", temp );
 			free( temp );
 		}
 		fprintf( mailer, "The Official Condor Homepage is "
 				 "http://www.cs.wisc.edu/condor\n" );
 	}
 
 	fflush(mailer);
 	 
 	prev_umask = umask(022);
 	 
 #if defined(WIN32)
 	if (EMAIL_FINAL_COMMAND == NULL) {
 		my_pclose( mailer );
 	} else {
 		char *email_filename = NULL;
 		  
 		fclose( mailer );
 		dprintf(D_FULLDEBUG,"Sending email via system(%s)\n",
 			EMAIL_FINAL_COMMAND);
 		system(EMAIL_FINAL_COMMAND);
 		if ( (email_filename=strrchr(EMAIL_FINAL_COMMAND,'<')) ) {
 			email_filename++;	 
 			email_filename++;	 
 			if ( unlink(email_filename) == -1 ) {
 				dprintf(D_ALWAYS,"email_close: cannot unlink temp file %s\n",
 					email_filename);
 			}
 		}
 		free(EMAIL_FINAL_COMMAND);
 		EMAIL_FINAL_COMMAND = NULL;
 	}
 #else
 	(void)fclose( mailer );
 #endif
 	umask(prev_umask);
 
 	 
 	set_priv(priv);
 
 }