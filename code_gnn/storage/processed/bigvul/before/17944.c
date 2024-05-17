choose_mac(struct ssh *ssh, struct sshmac *mac, char *client, char *server)
{
	char *name = match_list(client, server, NULL);

	if (name == NULL)
		return SSH_ERR_NO_MAC_ALG_MATCH;
	if (mac_setup(mac, name) < 0)
		return SSH_ERR_INTERNAL_ERROR;
	 
	if (ssh->compat & SSH_BUG_HMAC)
		mac->key_len = 16;
	mac->name = name;
	mac->key = NULL;
	mac->enabled = 0;
	return 0;
}