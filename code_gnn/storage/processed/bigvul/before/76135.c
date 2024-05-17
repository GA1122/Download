set_default_script_user(const char *username, const char *groupname)
{
	if (!default_script_uid_set || username) {
		 
		default_script_uid_set = true;

		if (set_uid_gid(username, groupname, &default_script_uid, &default_script_gid, true)) {
			if (username || script_security)
				default_user_fail = true;
		}
		else
			default_user_fail = false;
	}

	return default_user_fail;
}
