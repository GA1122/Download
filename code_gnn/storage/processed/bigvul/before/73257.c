int lxc_attach_run_shell(void* payload)
{
	uid_t uid;
	struct passwd *passwd;
	char *user_shell;

	 
	(void)payload;

	uid = getuid();
	passwd = getpwuid(uid);

	 
	if (!passwd)
		user_shell = lxc_attach_getpwshell(uid);
	else
		user_shell = passwd->pw_shell;

	if (user_shell)
		execlp(user_shell, user_shell, (char *)NULL);

	 
	execlp("/bin/sh", "/bin/sh", (char *)NULL);
	SYSERROR("failed to exec shell");
	return -1;
}
