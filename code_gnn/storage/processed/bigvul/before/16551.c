email_developers_open(const char *subject)
{
	char *tmp;
	FILE *mailer;

	 
    tmp = param ("CONDOR_DEVELOPERS");
    if (tmp == NULL) {
		 
#ifdef NO_PHONE_HOME
		tmp = strdup("NONE");
#else
        tmp = strdup("condor-admin@cs.wisc.edu");
#endif
    } else
    if (strcasecmp (tmp, "NONE") == 0) {
        free (tmp);
        return NULL;
    }

	mailer = email_open(tmp,subject);		

	 
	free(tmp);
	return mailer;
}
