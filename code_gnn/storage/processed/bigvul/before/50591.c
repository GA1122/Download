sys_auth_passwd(Authctxt *authctxt, const char *password)
{
	struct passwd *pw = authctxt->pw;
	char *encrypted_password, *salt = NULL;

	 
	char *pw_password = authctxt->valid ? shadow_pw(pw) : pw->pw_passwd;

	 
	if (strcmp(pw_password, "") == 0 && strcmp(password, "") == 0)
		return (1);

	 
	if (authctxt->valid && pw_password[0] && pw_password[1])
		salt = pw_password;
	encrypted_password = xcrypt(password, salt);

	 
	return encrypted_password != NULL &&
	    strcmp(encrypted_password, pw_password) == 0;
}
