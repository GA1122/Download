struct key *keyring_alloc(const char *description, kuid_t uid, kgid_t gid,
			  const struct cred *cred, key_perm_t perm,
			  unsigned long flags,
			  struct key_restriction *restrict_link,
			  struct key *dest)
{
	struct key *keyring;
	int ret;

	keyring = key_alloc(&key_type_keyring, description,
			    uid, gid, cred, perm, flags, restrict_link);
	if (!IS_ERR(keyring)) {
		ret = key_instantiate_and_link(keyring, NULL, 0, dest, NULL);
		if (ret < 0) {
			key_put(keyring);
			keyring = ERR_PTR(ret);
		}
	}

	return keyring;
}