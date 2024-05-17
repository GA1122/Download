identity_sign_encode(struct identity *id)
{
	struct ssh *ssh = active_state;

	if (id->key->type == KEY_RSA) {
		switch (ssh->kex->rsa_sha2) {
		case 256:
			return "rsa-sha2-256";
		case 512:
			return "rsa-sha2-512";
		}
	}
	return key_ssh_name(id->key);
}
