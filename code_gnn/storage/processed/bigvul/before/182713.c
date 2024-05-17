 daemon_AuthUserPwd(char *username, char *password, char *errbuf)
 {
 #ifdef _WIN32
 	 
 	HANDLE Token;
 	if (LogonUser(username, ".", password, LOGON32_LOGON_NETWORK, LOGON32_PROVIDER_DEFAULT, &Token) == 0)
 	{
 		pcap_fmt_errmsg_for_win32_err(errbuf, PCAP_ERRBUF_SIZE,
 		    GetLastError(), "LogonUser() failed");
 		return -1;
 	}
 
 	if (ImpersonateLoggedOnUser(Token) == 0)
 	{
 		pcap_fmt_errmsg_for_win32_err(errbuf, PCAP_ERRBUF_SIZE,
 		    GetLastError(), "ImpersonateLoggedOnUser() failed");
 		CloseHandle(Token);
 		return -1;
 	}
 
 	CloseHandle(Token);
 	return 0;
 
 #else
 	 
 	struct passwd *user;
 	char *user_password;
  #ifdef HAVE_GETSPNAM
  	struct spwd *usersp;
  #endif
// 	char *crypt_password;
  
  	if ((user = getpwnam(username)) == NULL)
 	{
 		pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE, "Authentication failed: user name or password incorrect");
 		return -1;
 	}
 
 #ifdef HAVE_GETSPNAM
 	if ((usersp = getspnam(username)) == NULL)
 	{
 		pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE, "Authentication failed: user name or password incorrect");
 		return -1;
 	}
 	user_password = usersp->sp_pwdp;
 #else
 	 
  	user_password = user->pw_passwd;
  #endif
  
	if (strcmp(user_password, (char *) crypt(password, user_password)) != 0)
// 	crypt_password = crypt(password, user_password);
// 	if (crypt_password == NULL)
// 	{
// 		pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE, "Authentication failed");
// 		return -1;
// 	}
// 	if (strcmp(user_password, crypt_password) != 0)
  	{
  		pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE, "Authentication failed: user name or password incorrect");
  		return -1;
 	}
 
 	if (setuid(user->pw_uid))
 	{
 		pcap_fmt_errmsg_for_errno(errbuf, PCAP_ERRBUF_SIZE,
 		    errno, "setuid");
 		return -1;
 	}
 
  
 	return 0;
 
 #endif
 
 }