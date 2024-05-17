static bool op_attr_secret_value_from_binary(void *attr, const void *data,
					     size_t data_len, size_t *offs)
{
	struct tee_cryp_obj_secret *key = attr;
	uint32_t s;

	if (!op_u32_from_binary_helper(&s, data, data_len, offs))
		return false;

	if ((*offs + s) > data_len)
		return false;

	 
	if (s > key->alloc_size)
		return false;
	key->key_size = s;
	memcpy(key + 1, (const uint8_t *)data + *offs, s);
	(*offs) += s;
	return true;
}