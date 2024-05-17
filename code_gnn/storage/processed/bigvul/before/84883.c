static void ascii_ssetup_strings(char **pbcc_area, struct cifs_ses *ses,
				 const struct nls_table *nls_cp)
{
	char *bcc_ptr = *pbcc_area;

	 
	 
	 
	if (ses->user_name != NULL) {
		strncpy(bcc_ptr, ses->user_name, CIFS_MAX_USERNAME_LEN);
		bcc_ptr += strnlen(ses->user_name, CIFS_MAX_USERNAME_LEN);
	}
	 
	*bcc_ptr = 0;
	bcc_ptr++;  

	 
	if (ses->domainName != NULL) {
		strncpy(bcc_ptr, ses->domainName, CIFS_MAX_DOMAINNAME_LEN);
		bcc_ptr += strnlen(ses->domainName, CIFS_MAX_DOMAINNAME_LEN);
	}  
	*bcc_ptr = 0;
	bcc_ptr++;

	 

	strcpy(bcc_ptr, "Linux version ");
	bcc_ptr += strlen("Linux version ");
	strcpy(bcc_ptr, init_utsname()->release);
	bcc_ptr += strlen(init_utsname()->release) + 1;

	strcpy(bcc_ptr, CIFS_NETWORK_OPSYS);
	bcc_ptr += strlen(CIFS_NETWORK_OPSYS) + 1;

	*pbcc_area = bcc_ptr;
}
