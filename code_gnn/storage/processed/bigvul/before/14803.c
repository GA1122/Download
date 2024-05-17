ftp_pwd(ftpbuf_t *ftp)
{
	char *pwd, *end;

	if (ftp == NULL) {
		return NULL;
	}

	 
	if (ftp->pwd) {
		return ftp->pwd;
	}
	if (!ftp_putcmd(ftp, "PWD", NULL)) {
		return NULL;
	}
	if (!ftp_getresp(ftp) || ftp->resp != 257) {
		return NULL;
	}
	 
	if ((pwd = strchr(ftp->inbuf, '"')) == NULL) {
		return NULL;
	}
	if ((end = strrchr(++pwd, '"')) == NULL) {
		return NULL;
	}
	ftp->pwd = estrndup(pwd, end - pwd);

	return ftp->pwd;
}
