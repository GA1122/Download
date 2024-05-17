static TEE_Result op_attr_secret_value_from_user(void *attr, const void *buffer,
						 size_t size)
{
	struct tee_cryp_obj_secret *key = attr;

	 
	if (size > key->alloc_size)
		return TEE_ERROR_SECURITY;
	memcpy(key + 1, buffer, size);
	key->key_size = size;
	return TEE_SUCCESS;
}
