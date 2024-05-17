static TEE_Result op_attr_value_from_user(void *attr, const void *buffer,
					  size_t size)
{
	uint32_t *v = attr;

	if (size != sizeof(uint32_t) * 2)
		return TEE_ERROR_GENERIC;  

	 
	memcpy(v, buffer, sizeof(uint32_t));
	return TEE_SUCCESS;
}
