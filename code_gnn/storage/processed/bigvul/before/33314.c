key_ref_t search_my_process_keyrings(struct key_type *type,
				     const void *description,
				     key_match_func_t match,
				     bool no_state_check,
				     const struct cred *cred)
{
	key_ref_t key_ref, ret, err;

	 
	key_ref = NULL;
	ret = NULL;
	err = ERR_PTR(-EAGAIN);

	 
	if (cred->thread_keyring) {
		key_ref = keyring_search_aux(
			make_key_ref(cred->thread_keyring, 1),
			cred, type, description, match, no_state_check);
		if (!IS_ERR(key_ref))
			goto found;

		switch (PTR_ERR(key_ref)) {
		case -EAGAIN:  
		case -ENOKEY:  
			ret = key_ref;
			break;
		default:
			err = key_ref;
			break;
		}
	}

	 
	if (cred->process_keyring) {
		key_ref = keyring_search_aux(
			make_key_ref(cred->process_keyring, 1),
			cred, type, description, match, no_state_check);
		if (!IS_ERR(key_ref))
			goto found;

		switch (PTR_ERR(key_ref)) {
		case -EAGAIN:  
			if (ret)
				break;
		case -ENOKEY:  
			ret = key_ref;
			break;
		default:
			err = key_ref;
			break;
		}
	}

	 
	if (cred->session_keyring) {
		rcu_read_lock();
		key_ref = keyring_search_aux(
			make_key_ref(rcu_dereference(cred->session_keyring), 1),
			cred, type, description, match, no_state_check);
		rcu_read_unlock();

		if (!IS_ERR(key_ref))
			goto found;

		switch (PTR_ERR(key_ref)) {
		case -EAGAIN:  
			if (ret)
				break;
		case -ENOKEY:  
			ret = key_ref;
			break;
		default:
			err = key_ref;
			break;
		}
	}
	 
	else if (cred->user->session_keyring) {
		key_ref = keyring_search_aux(
			make_key_ref(cred->user->session_keyring, 1),
			cred, type, description, match, no_state_check);
		if (!IS_ERR(key_ref))
			goto found;

		switch (PTR_ERR(key_ref)) {
		case -EAGAIN:  
			if (ret)
				break;
		case -ENOKEY:  
			ret = key_ref;
			break;
		default:
			err = key_ref;
			break;
		}
	}

	 
	key_ref = ret ? ret : err;

found:
	return key_ref;
}
