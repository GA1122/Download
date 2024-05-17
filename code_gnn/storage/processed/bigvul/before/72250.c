input_userauth_pk_ok(int type, u_int32_t seq, void *ctxt)
{
	Authctxt *authctxt = ctxt;
	Key *key = NULL;
	Identity *id = NULL;
	Buffer b;
	int pktype, sent = 0;
	u_int alen, blen;
	char *pkalg, *fp;
	u_char *pkblob;

	if (authctxt == NULL)
		fatal("input_userauth_pk_ok: no authentication context");
	if (datafellows & SSH_BUG_PKOK) {
		 
		debug2("input_userauth_pk_ok: SSH_BUG_PKOK");
		pkblob = packet_get_string(&blen);
		buffer_init(&b);
		buffer_append(&b, pkblob, blen);
		pkalg = buffer_get_string(&b, &alen);
		buffer_free(&b);
	} else {
		pkalg = packet_get_string(&alen);
		pkblob = packet_get_string(&blen);
	}
	packet_check_eom();

	debug("Server accepts key: pkalg %s blen %u", pkalg, blen);

	if ((pktype = key_type_from_name(pkalg)) == KEY_UNSPEC) {
		debug("unknown pkalg %s", pkalg);
		goto done;
	}
	if ((key = key_from_blob(pkblob, blen)) == NULL) {
		debug("no key from blob. pkalg %s", pkalg);
		goto done;
	}
	if (key->type != pktype) {
		error("input_userauth_pk_ok: type mismatch "
		    "for decoded key (received %d, expected %d)",
		    key->type, pktype);
		goto done;
	}
	if ((fp = sshkey_fingerprint(key, options.fingerprint_hash,
	    SSH_FP_DEFAULT)) == NULL)
		goto done;
	debug2("input_userauth_pk_ok: fp %s", fp);
	free(fp);

	 
	TAILQ_FOREACH_REVERSE(id, &authctxt->keys, idlist, next) {
		if (key_equal(key, id->key)) {
			sent = sign_and_send_pubkey(authctxt, id);
			break;
		}
	}
done:
	if (key != NULL)
		key_free(key);
	free(pkalg);
	free(pkblob);

	 
	if (sent == 0)
		userauth(authctxt, NULL);
	return 0;
}
