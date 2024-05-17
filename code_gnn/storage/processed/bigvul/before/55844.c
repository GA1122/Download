key_ref_t search_my_process_keyrings(struct keyring_search_context *ctx)
{
	key_ref_t key_ref, ret, err;

	 
	key_ref = NULL;
	ret = NULL;
	err = ERR_PTR(-EAGAIN);

	 
	if (ctx->cred->thread_keyring) {
		key_ref = keyring_search_aux(
			make_key_ref(ctx->cred->thread_keyring, 1), ctx);
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

	 
	if (ctx->cred->process_keyring) {
		key_ref = keyring_search_aux(
			make_key_ref(ctx->cred->process_keyring, 1), ctx);
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

	 
	if (ctx->cred->session_keyring) {
		rcu_read_lock();
		key_ref = keyring_search_aux(
			make_key_ref(rcu_dereference(ctx->cred->session_keyring), 1),
			ctx);
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
	 
	else if (ctx->cred->user->session_keyring) {
		key_ref = keyring_search_aux(
			make_key_ref(ctx->cred->user->session_keyring, 1),
			ctx);
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
