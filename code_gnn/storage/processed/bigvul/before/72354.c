process_remove_identity(SocketEntry *e, int version)
{
	size_t blen;
	int r, success = 0;
	struct sshkey *key = NULL;
	u_char *blob;
#ifdef WITH_SSH1
	u_int bits;
#endif  

	switch (version) {
#ifdef WITH_SSH1
	case 1:
		if ((key = sshkey_new(KEY_RSA1)) == NULL) {
			error("%s: sshkey_new failed", __func__);
			return;
		}
		if ((r = sshbuf_get_u32(e->request, &bits)) != 0 ||
		    (r = sshbuf_get_bignum1(e->request, key->rsa->e)) != 0 ||
		    (r = sshbuf_get_bignum1(e->request, key->rsa->n)) != 0)
			fatal("%s: buffer error: %s", __func__, ssh_err(r));

		if (bits != sshkey_size(key))
			logit("Warning: identity keysize mismatch: "
			    "actual %u, announced %u",
			    sshkey_size(key), bits);
		break;
#endif  
	case 2:
		if ((r = sshbuf_get_string(e->request, &blob, &blen)) != 0)
			fatal("%s: buffer error: %s", __func__, ssh_err(r));
		if ((r = sshkey_from_blob(blob, blen, &key)) != 0)
			error("%s: sshkey_from_blob failed: %s",
			    __func__, ssh_err(r));
		free(blob);
		break;
	}
	if (key != NULL) {
		Identity *id = lookup_identity(key, version);
		if (id != NULL) {
			 
			Idtab *tab = idtab_lookup(version);
			if (tab->nentries < 1)
				fatal("process_remove_identity: "
				    "internal error: tab->nentries %d",
				    tab->nentries);
			TAILQ_REMOVE(&tab->idlist, id, next);
			free_identity(id);
			tab->nentries--;
			success = 1;
		}
		sshkey_free(key);
	}
	send_status(e, success);
}