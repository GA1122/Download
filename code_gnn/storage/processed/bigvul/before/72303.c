sshkey_check_revoked(struct sshkey *key, const char *revoked_keys_file)
{
	int r;

	r = ssh_krl_file_contains_key(revoked_keys_file, key);
	 
	if (r != SSH_ERR_KRL_BAD_MAGIC)
		return r;

	 
	switch ((r = sshkey_in_file(key, revoked_keys_file, 0, 1))) {
	case 0:
		 
		return SSH_ERR_KEY_REVOKED;
	case SSH_ERR_KEY_NOT_FOUND:
		 
		return 0;
	default:
		 
		return r;
	}
}
