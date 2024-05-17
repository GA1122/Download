static TEE_Result op_attr_secret_value_from_obj(void *attr, void *src_attr)
{
	struct tee_cryp_obj_secret *key = attr;
	struct tee_cryp_obj_secret *src_key = src_attr;

	if (src_key->key_size > key->alloc_size)
		return TEE_ERROR_BAD_STATE;
	memcpy(key + 1, src_key + 1, src_key->key_size);
	key->key_size = src_key->key_size;
	return TEE_SUCCESS;
}