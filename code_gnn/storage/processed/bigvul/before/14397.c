launch_login(struct passwd *pw, const char *hostname)
{
	 

	execl(LOGIN_PROGRAM, "login", "-h", hostname,
#ifdef xxxLOGIN_NEEDS_TERM
		    (s->term ? s->term : "unknown"),
#endif  
#ifdef LOGIN_NO_ENDOPT
	    "-p", "-f", pw->pw_name, (char *)NULL);
#else
	    "-p", "-f", "--", pw->pw_name, (char *)NULL);
#endif

	 

	perror("login");
	exit(1);
}
