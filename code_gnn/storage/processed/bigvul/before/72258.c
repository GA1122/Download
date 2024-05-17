pubkey_prepare(Authctxt *authctxt)
{
	struct identity *id, *id2, *tmp;
	struct idlist agent, files, *preferred;
	struct sshkey *key;
	int agent_fd = -1, i, r, found;
	size_t j;
	struct ssh_identitylist *idlist;

	TAILQ_INIT(&agent);	 
	TAILQ_INIT(&files);	 
	preferred = &authctxt->keys;
	TAILQ_INIT(preferred);	 

	 
	for (i = 0; i < options.num_identity_files; i++) {
		key = options.identity_keys[i];
		if (key && key->type == KEY_RSA1)
			continue;
		if (key && key->cert && key->cert->type != SSH2_CERT_TYPE_USER)
			continue;
		options.identity_keys[i] = NULL;
		id = xcalloc(1, sizeof(*id));
		id->agent_fd = -1;
		id->key = key;
		id->filename = xstrdup(options.identity_files[i]);
		id->userprovided = options.identity_file_userprovided[i];
		TAILQ_INSERT_TAIL(&files, id, next);
	}
	 
	for (i = 0; i < options.num_certificate_files; i++) {
		key = options.certificates[i];
		if (!key_is_cert(key) || key->cert == NULL ||
		    key->cert->type != SSH2_CERT_TYPE_USER)
			continue;
		id = xcalloc(1, sizeof(*id));
		id->agent_fd = -1;
		id->key = key;
		id->filename = xstrdup(options.certificate_files[i]);
		id->userprovided = options.certificate_file_userprovided[i];
		TAILQ_INSERT_TAIL(preferred, id, next);
	}
	 
	if ((r = ssh_get_authentication_socket(&agent_fd)) != 0) {
		if (r != SSH_ERR_AGENT_NOT_PRESENT)
			debug("%s: ssh_get_authentication_socket: %s",
			    __func__, ssh_err(r));
	} else if ((r = ssh_fetch_identitylist(agent_fd, 2, &idlist)) != 0) {
		if (r != SSH_ERR_AGENT_NO_IDENTITIES)
			debug("%s: ssh_fetch_identitylist: %s",
			    __func__, ssh_err(r));
		close(agent_fd);
	} else {
		for (j = 0; j < idlist->nkeys; j++) {
			found = 0;
			TAILQ_FOREACH(id, &files, next) {
				 
				if (sshkey_equal(idlist->keys[j], id->key)) {
					TAILQ_REMOVE(&files, id, next);
					TAILQ_INSERT_TAIL(preferred, id, next);
					id->agent_fd = agent_fd;
					found = 1;
					break;
				}
			}
			if (!found && !options.identities_only) {
				id = xcalloc(1, sizeof(*id));
				 
				id->key = idlist->keys[j];
				id->filename = idlist->comments[j];
				idlist->keys[j] = NULL;
				idlist->comments[j] = NULL;
				id->agent_fd = agent_fd;
				TAILQ_INSERT_TAIL(&agent, id, next);
			}
		}
		ssh_free_identitylist(idlist);
		 
		for (id = TAILQ_FIRST(&agent); id; id = TAILQ_FIRST(&agent)) {
			TAILQ_REMOVE(&agent, id, next);
			TAILQ_INSERT_TAIL(preferred, id, next);
		}
		authctxt->agent_fd = agent_fd;
	}
	 
	TAILQ_FOREACH_SAFE(id, &files, next, tmp) {
		if (id->key == NULL || (id->key->flags & SSHKEY_FLAG_EXT) == 0)
			continue;
		found = 0;
		TAILQ_FOREACH(id2, &files, next) {
			if (id2->key == NULL ||
			    (id2->key->flags & SSHKEY_FLAG_EXT) == 0)
				continue;
			if (sshkey_equal(id->key, id2->key)) {
				TAILQ_REMOVE(&files, id, next);
				TAILQ_INSERT_TAIL(preferred, id, next);
				found = 1;
				break;
			}
		}
		 
		if (!found && options.identities_only) {
			TAILQ_REMOVE(&files, id, next);
			explicit_bzero(id, sizeof(*id));
			free(id);
		}
	}
	 
	for (id = TAILQ_FIRST(&files); id; id = TAILQ_FIRST(&files)) {
		TAILQ_REMOVE(&files, id, next);
		TAILQ_INSERT_TAIL(preferred, id, next);
	}
	 
	TAILQ_FOREACH_SAFE(id, preferred, next, id2) {
		if (id->key != NULL &&
		    match_pattern_list(sshkey_ssh_name(id->key),
		    options.pubkey_key_types, 0) != 1) {
			debug("Skipping %s key %s - "
			    "not in PubkeyAcceptedKeyTypes",
			    sshkey_ssh_name(id->key), id->filename);
			TAILQ_REMOVE(preferred, id, next);
			sshkey_free(id->key);
			free(id->filename);
			memset(id, 0, sizeof(*id));
			continue;
		}
		debug2("key: %s (%p)%s%s", id->filename, id->key,
		    id->userprovided ? ", explicit" : "",
		    id->agent_fd != -1 ? ", agent" : "");
	}
}
