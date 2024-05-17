process_add_identity(SocketEntry *e, int version)
{
	Idtab *tab = idtab_lookup(version);
	Identity *id;
	int success = 0, confirm = 0;
	u_int seconds;
	char *comment = NULL;
	time_t death = 0;
	struct sshkey *k = NULL;
	u_char ctype;
	int r = SSH_ERR_INTERNAL_ERROR;

	switch (version) {
#ifdef WITH_SSH1
	case 1:
		r = agent_decode_rsa1(e->request, &k);
		break;
#endif  
	case 2:
		r = sshkey_private_deserialize(e->request, &k);
		break;
	}
	if (r != 0 || k == NULL ||
	    (r = sshbuf_get_cstring(e->request, &comment, NULL)) != 0) {
		error("%s: decode private key: %s", __func__, ssh_err(r));
		goto err;
	}

	while (sshbuf_len(e->request)) {
		if ((r = sshbuf_get_u8(e->request, &ctype)) != 0) {
			error("%s: buffer error: %s", __func__, ssh_err(r));
			goto err;
		}
		switch (ctype) {
		case SSH_AGENT_CONSTRAIN_LIFETIME:
			if ((r = sshbuf_get_u32(e->request, &seconds)) != 0) {
				error("%s: bad lifetime constraint: %s",
				    __func__, ssh_err(r));
				goto err;
			}
			death = monotime() + seconds;
			break;
		case SSH_AGENT_CONSTRAIN_CONFIRM:
			confirm = 1;
			break;
		default:
			error("%s: Unknown constraint %d", __func__, ctype);
 err:
			sshbuf_reset(e->request);
			free(comment);
			sshkey_free(k);
			goto send;
		}
	}

	success = 1;
	if (lifetime && !death)
		death = monotime() + lifetime;
	if ((id = lookup_identity(k, version)) == NULL) {
		id = xcalloc(1, sizeof(Identity));
		id->key = k;
		TAILQ_INSERT_TAIL(&tab->idlist, id, next);
		 
		tab->nentries++;
	} else {
		sshkey_free(k);
		free(id->comment);
	}
	id->comment = comment;
	id->death = death;
	id->confirm = confirm;
send:
	send_status(e, success);
}