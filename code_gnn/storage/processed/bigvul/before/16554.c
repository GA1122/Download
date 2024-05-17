email_open_implementation( const char * final_args[])
{

	FILE *mailerstream;
	pid_t pid;
	int pipefds[2];
	param_functions *p_funcs = NULL;

	 

	if (pipe(pipefds) < 0)
	{
		dprintf(D_ALWAYS, "Could not open email pipe!\n");
		return NULL;
	}

	dprintf(D_FULLDEBUG, "Forking Mailer process...\n");
	if ((pid = fork()) < 0)
	{
		dprintf(D_ALWAYS, "Could not fork email process!\n");
		return NULL;
	}
	else if (pid > 0)  
	{
		 

		 
		close(pipefds[0]);

		mailerstream = fdopen(pipefds[1], EMAIL_POPEN_FLAGS);
		if (mailerstream == NULL)
		{
			dprintf(D_ALWAYS, "Could not open email FILE*: %s\n", 
				strerror(errno));
			return NULL;
		}
		return mailerstream;
	}
	else  
	{
		static char pe_logname[256];  
		static char pe_user[256];
		const char *condor_name;
		int i;

		 
		_EXCEPT_Cleanup = NULL;

		 
		Termlog = 1;
		p_funcs = get_param_functions();
		dprintf_config(get_mySubSystemName(), p_funcs);

		 
		chdir("/");
		umask(0);

		 
		 
		set_condor_priv_final();

		 
		close(pipefds[1]);

		 
		if (dup2(pipefds[0], STDIN_FILENO) < 0)
		{
			 
			EXCEPT("EMAIL PROCESS: Could not connect stdin to child!\n");
		}

		 
		for(i = 0; i < sysconf(_SC_OPEN_MAX); i++)
		{
			if (i != pipefds[0] && i != STDIN_FILENO)
			{
				(void)close(i);
			}
		}

		 
		condor_name = get_condor_username();

		 
		sprintf(pe_logname,"LOGNAME=%s", condor_name);
		if (putenv(pe_logname) != 0)
		{
			EXCEPT("EMAIL PROCESS: Unable to insert LOGNAME=%s into "
				" environment correctly: %s\n", pe_logname, strerror(errno));
		}

		 
		sprintf(pe_user,"USER=%s", condor_name);
		if( putenv(pe_user) != 0)
		{
			 
			EXCEPT("EMAIL PROCESS: Unable to insert USER=%s into "
				" environment correctly: %s\n", pe_user, strerror(errno));
		}

		 
		execvp(final_args[0], const_cast<char *const*>(final_args) );

		 
		EXCEPT("EMAIL PROCESS: Could not exec mailer using '%s' with command "
			"'%s' because of error: %s.", "/bin/sh", 
			(final_args[0]==NULL)?"(null)":final_args[0], strerror(errno));
	}

	 
	return NULL;
}
