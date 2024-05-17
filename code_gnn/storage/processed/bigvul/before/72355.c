process_remove_smartcard_key(SocketEntry *e)
{
	char *provider = NULL, *pin = NULL;
	int r, version, success = 0;
	Identity *id, *nxt;
	Idtab *tab;

	if ((r = sshbuf_get_cstring(e->request, &provider, NULL)) != 0 ||
	    (r = sshbuf_get_cstring(e->request, &pin, NULL)) != 0)
		fatal("%s: buffer error: %s", __func__, ssh_err(r));
	free(pin);

	for (version = 1; version < 3; version++) {
		tab = idtab_lookup(version);
		for (id = TAILQ_FIRST(&tab->idlist); id; id = nxt) {
			nxt = TAILQ_NEXT(id, next);
			 
			if (id->provider == NULL)
				continue;
			if (!strcmp(provider, id->provider)) {
				TAILQ_REMOVE(&tab->idlist, id, next);
				free_identity(id);
				tab->nentries--;
			}
		}
	}
	if (pkcs11_del_provider(provider) == 0)
		success = 1;
	else
		error("process_remove_smartcard_key:"
		    " pkcs11_del_provider failed");
	free(provider);
	send_status(e, success);
}