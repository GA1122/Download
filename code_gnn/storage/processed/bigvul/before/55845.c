key_ref_t search_process_keyrings(struct keyring_search_context *ctx)
{
	struct request_key_auth *rka;
	key_ref_t key_ref, ret = ERR_PTR(-EACCES), err;

	might_sleep();

	key_ref = search_my_process_keyrings(ctx);
	if (!IS_ERR(key_ref))
		goto found;
	err = key_ref;

	 
	if (ctx->cred->request_key_auth &&
	    ctx->cred == current_cred() &&
	    ctx->index_key.type != &key_type_request_key_auth
	    ) {
		const struct cred *cred = ctx->cred;

		 
		down_read(&cred->request_key_auth->sem);

		if (key_validate(ctx->cred->request_key_auth) == 0) {
			rka = ctx->cred->request_key_auth->payload.data[0];

			ctx->cred = rka->cred;
			key_ref = search_process_keyrings(ctx);
			ctx->cred = cred;

			up_read(&cred->request_key_auth->sem);

			if (!IS_ERR(key_ref))
				goto found;

			ret = key_ref;
		} else {
			up_read(&cred->request_key_auth->sem);
		}
	}

	 
	if (err == ERR_PTR(-ENOKEY) || ret == ERR_PTR(-ENOKEY))
		key_ref = ERR_PTR(-ENOKEY);
	else if (err == ERR_PTR(-EACCES))
		key_ref = ret;
	else
		key_ref = err;

found:
	return key_ref;
}
