long keyctl_get_security(key_serial_t keyid,
			 char __user *buffer,
			 size_t buflen)
{
	struct key *key, *instkey;
	key_ref_t key_ref;
	char *context;
	long ret;

	key_ref = lookup_user_key(keyid, KEY_LOOKUP_PARTIAL, KEY_NEED_VIEW);
	if (IS_ERR(key_ref)) {
		if (PTR_ERR(key_ref) != -EACCES)
			return PTR_ERR(key_ref);

		 
		instkey = key_get_instantiation_authkey(keyid);
		if (IS_ERR(instkey))
			return PTR_ERR(instkey);
		key_put(instkey);

		key_ref = lookup_user_key(keyid, KEY_LOOKUP_PARTIAL, 0);
		if (IS_ERR(key_ref))
			return PTR_ERR(key_ref);
	}

	key = key_ref_to_ptr(key_ref);
	ret = security_key_getsecurity(key, &context);
	if (ret == 0) {
		 
		ret = 1;
		if (buffer && buflen > 0 &&
		    copy_to_user(buffer, "", 1) != 0)
			ret = -EFAULT;
	} else if (ret > 0) {
		 
		if (buffer && buflen > 0) {
			if (buflen > ret)
				buflen = ret;

			if (copy_to_user(buffer, context, buflen) != 0)
				ret = -EFAULT;
		}

		kfree(context);
	}

	key_ref_put(key_ref);
	return ret;
}
