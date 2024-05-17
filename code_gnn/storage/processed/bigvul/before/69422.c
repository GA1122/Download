cifs_idmap_key_instantiate(struct key *key, struct key_preparsed_payload *prep)
{
	char *payload;

	 
	if (prep->datalen <= sizeof(key->payload)) {
		key->payload.value = 0;
		memcpy(&key->payload.value, prep->data, prep->datalen);
		key->datalen = prep->datalen;
		return 0;
	}
	payload = kmemdup(prep->data, prep->datalen, GFP_KERNEL);
	if (!payload)
		return -ENOMEM;

	key->payload.data = payload;
	key->datalen = prep->datalen;
	return 0;
}
