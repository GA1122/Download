static int match_session(struct cifs_ses *ses, struct smb_vol *vol)
{
	switch (ses->server->secType) {
	case Kerberos:
		if (!uid_eq(vol->cred_uid, ses->cred_uid))
			return 0;
		break;
	default:
		 
		if (ses->user_name == NULL) {
			if (!vol->nullauth)
				return 0;
			break;
		}

		 
		if (strncmp(ses->user_name,
			    vol->username ? vol->username : "",
			    MAX_USERNAME_SIZE))
			return 0;
		if (strlen(vol->username) != 0 &&
		    ses->password != NULL &&
		    strncmp(ses->password,
			    vol->password ? vol->password : "",
			    MAX_PASSWORD_SIZE))
			return 0;
	}
	return 1;
}
