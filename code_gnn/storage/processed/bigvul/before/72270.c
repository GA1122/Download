userauth_pubkey(Authctxt *authctxt)
{
	Identity *id;
	int sent = 0;

	while ((id = TAILQ_FIRST(&authctxt->keys))) {
		if (id->tried++)
			return (0);
		 
		TAILQ_REMOVE(&authctxt->keys, id, next);
		TAILQ_INSERT_TAIL(&authctxt->keys, id, next);
		 
		if (id->key != NULL) {
			if (try_identity(id)) {
				debug("Offering %s public key: %s",
				    key_type(id->key), id->filename);
				sent = send_pubkey_test(authctxt, id);
			}
		} else {
			debug("Trying private key: %s", id->filename);
			id->key = load_identity_file(id);
			if (id->key != NULL) {
				if (try_identity(id)) {
					id->isprivate = 1;
					sent = sign_and_send_pubkey(
					    authctxt, id);
				}
				key_free(id->key);
				id->key = NULL;
			}
		}
		if (sent)
			return (sent);
	}
	return (0);
}
