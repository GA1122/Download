int __key_link_check_live_key(struct key *keyring, struct key *key)
{
	if (key->type == &key_type_keyring)
		 
		return keyring_detect_cycle(keyring, key);
	return 0;
}
