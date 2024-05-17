static int keyring_instantiate(struct key *keyring,
			       struct key_preparsed_payload *prep)
{
	assoc_array_init(&keyring->keys);
	 
	keyring_publish_name(keyring);
	return 0;
}
