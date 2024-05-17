long keyctl_assume_authority(key_serial_t id)
{
	struct key *authkey;
	long ret;

	 
	ret = -EINVAL;
	if (id < 0)
		goto error;

	 
	if (id == 0) {
		ret = keyctl_change_reqkey_auth(NULL);
		goto error;
	}

	 
	authkey = key_get_instantiation_authkey(id);
	if (IS_ERR(authkey)) {
		ret = PTR_ERR(authkey);
		goto error;
	}

	ret = keyctl_change_reqkey_auth(authkey);
	if (ret < 0)
		goto error;
	key_put(authkey);

	ret = authkey->serial;
error:
	return ret;
}
