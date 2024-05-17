static TEE_Result get_hkdf_params(const TEE_Attribute *params,
				  uint32_t param_count,
				  void **salt, size_t *salt_len, void **info,
				  size_t *info_len, size_t *okm_len)
{
	size_t n;
	enum { SALT = 0x1, LENGTH = 0x2, INFO = 0x4 };
	uint8_t found = 0;

	*salt = *info = NULL;
	*salt_len = *info_len = *okm_len = 0;

	for (n = 0; n < param_count; n++) {
		switch (params[n].attributeID) {
		case TEE_ATTR_HKDF_SALT:
			if (!(found & SALT)) {
				*salt = params[n].content.ref.buffer;
				*salt_len = params[n].content.ref.length;
				found |= SALT;
			}
			break;
		case TEE_ATTR_HKDF_OKM_LENGTH:
			if (!(found & LENGTH)) {
				*okm_len = params[n].content.value.a;
				found |= LENGTH;
			}
			break;
		case TEE_ATTR_HKDF_INFO:
			if (!(found & INFO)) {
				*info = params[n].content.ref.buffer;
				*info_len = params[n].content.ref.length;
				found |= INFO;
			}
			break;
		default:
			 
			return TEE_ERROR_BAD_PARAMETERS;
		}

	}

	if (!(found & LENGTH))
		return TEE_ERROR_BAD_PARAMETERS;

	return TEE_SUCCESS;
}