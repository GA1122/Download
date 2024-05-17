static TEE_Result get_pbkdf2_params(const TEE_Attribute *params,
				   uint32_t param_count, void **salt,
				   size_t *salt_len, size_t *derived_key_len,
				   size_t *iteration_count)
{
	size_t n;
	enum { SALT = 0x1, LENGTH = 0x2, COUNT = 0x4 };
	uint8_t found = 0;

	*salt = NULL;
	*salt_len = *derived_key_len = *iteration_count = 0;

	for (n = 0; n < param_count; n++) {
		switch (params[n].attributeID) {
		case TEE_ATTR_PBKDF2_SALT:
			if (!(found & SALT)) {
				*salt = params[n].content.ref.buffer;
				*salt_len = params[n].content.ref.length;
				found |= SALT;
			}
			break;
		case TEE_ATTR_PBKDF2_DKM_LENGTH:
			if (!(found & LENGTH)) {
				*derived_key_len = params[n].content.value.a;
				found |= LENGTH;
			}
			break;
		case TEE_ATTR_PBKDF2_ITERATION_COUNT:
			if (!(found & COUNT)) {
				*iteration_count = params[n].content.value.a;
				found |= COUNT;
			}
			break;
		default:
			 
			return TEE_ERROR_BAD_PARAMETERS;
		}
	}

	if ((found & (LENGTH|COUNT)) != (LENGTH|COUNT))
		return TEE_ERROR_BAD_PARAMETERS;

	return TEE_SUCCESS;
}