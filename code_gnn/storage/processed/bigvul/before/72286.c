list_hostkey_types(void)
{
	Buffer b;
	const char *p;
	char *ret;
	int i;
	Key *key;

	buffer_init(&b);
	for (i = 0; i < options.num_host_key_files; i++) {
		key = sensitive_data.host_keys[i];
		if (key == NULL)
			key = sensitive_data.host_pubkeys[i];
		if (key == NULL)
			continue;
		 
		if (match_pattern_list(sshkey_ssh_name(key),
		    options.hostkeyalgorithms, 0) != 1) {
			debug3("%s: %s key not permitted by HostkeyAlgorithms",
			    __func__, sshkey_ssh_name(key));
			continue;
		}
		switch (key->type) {
		case KEY_RSA:
		case KEY_DSA:
		case KEY_ECDSA:
		case KEY_ED25519:
			if (buffer_len(&b) > 0)
				buffer_append(&b, ",", 1);
			p = key_ssh_name(key);
			buffer_append(&b, p, strlen(p));

			 
			if (key->type == KEY_RSA) {
				p = ",rsa-sha2-512,rsa-sha2-256";
				buffer_append(&b, p, strlen(p));
			}
			break;
		}
		 
		key = sensitive_data.host_certificates[i];
		if (key == NULL)
			continue;
		switch (key->type) {
		case KEY_RSA_CERT:
		case KEY_DSA_CERT:
		case KEY_ECDSA_CERT:
		case KEY_ED25519_CERT:
			if (buffer_len(&b) > 0)
				buffer_append(&b, ",", 1);
			p = key_ssh_name(key);
			buffer_append(&b, p, strlen(p));
			break;
		}
	}
	if ((ret = sshbuf_dup_string(&b)) == NULL)
		fatal("%s: sshbuf_dup_string failed", __func__);
	buffer_free(&b);
	debug("list_hostkey_types: %s", ret);
	return ret;
}
