static int parse_username(char *rawuser, struct parsed_mount_info *parsed_info)
{
	char *user, *password, slash;
	int rc = 0;

	 
	password = strchr(rawuser, '%');
	if (password) {
		rc = set_password(parsed_info, password + 1);
		if (rc)
			return rc;
		*password = '\0';
	}

	 
	user = strchr(rawuser, '/');
	if (!user)
		user = strchr(rawuser, '\\');

	 
	if (user) {
		slash = *user;
		*user = '\0';
		strlcpy(parsed_info->domain, rawuser,
			sizeof(parsed_info->domain));
		*(user++) = slash;
	} else {
		user = rawuser;
	}

	strlcpy(parsed_info->username, user, sizeof(parsed_info->username));
	parsed_info->got_user = 1;
	if (password)
		*password = '%';

	return 0;
}
