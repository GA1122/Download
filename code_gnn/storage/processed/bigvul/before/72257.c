pubkey_cleanup(Authctxt *authctxt)
{
	Identity *id;

	if (authctxt->agent_fd != -1)
		ssh_close_authentication_socket(authctxt->agent_fd);
	for (id = TAILQ_FIRST(&authctxt->keys); id;
	    id = TAILQ_FIRST(&authctxt->keys)) {
		TAILQ_REMOVE(&authctxt->keys, id, next);
		sshkey_free(id->key);
		free(id->filename);
		free(id);
	}
}
