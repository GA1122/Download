static TEE_Result get_concat_kdf_params(const TEE_Attribute *params,
					uint32_t param_count,
					void **other_info,
					size_t *other_info_len,
					size_t *derived_key_len)
{
	size_t n;
	enum { LENGTH = 0x1, INFO = 0x2 };
	uint8_t found = 0;

	*other_info = NULL;
	*other_info_len = *derived_key_len = 0;

	for (n = 0; n < param_count; n++) {
		switch (params[n].attributeID) {
		case TEE_ATTR_CONCAT_KDF_OTHER_INFO:
			if (!(found & INFO)) {
				*other_info = params[n].content.ref.buffer;
				*other_info_len = params[n].content.ref.length;
				found |= INFO;
			}
			break;
		case TEE_ATTR_CONCAT_KDF_DKM_LENGTH:
			if (!(found & LENGTH)) {
				*derived_key_len = params[n].content.value.a;
				found |= LENGTH;
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