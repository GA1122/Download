	__acquires(key_serial_lock)
{
	key_serial_t pos = *_pos;
	struct key *key;

	spin_lock(&key_serial_lock);

	if (*_pos > INT_MAX)
		return NULL;
	key = find_ge_key(p, pos);
	if (!key)
		return NULL;
	*_pos = key->serial;
	return &key->serial_node;
}
