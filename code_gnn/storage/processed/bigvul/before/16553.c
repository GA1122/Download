email_open_implementation(char *Mailer, const char * final_args[])
{
	priv_state priv;
	int prev_umask;
	FILE *mailerstream;

	 
	priv = set_condor_priv();
	 
	prev_umask = umask(022);
	mailerstream = my_popenv(final_args,EMAIL_POPEN_FLAGS,FALSE);
	umask(prev_umask);

	 
	set_priv(priv);

	if ( mailerstream == NULL ) {	
		dprintf(D_ALWAYS,"Failed to access email program \"%s\"\n",
			Mailer);
	}

	return mailerstream;
}
