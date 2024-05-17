coolkey_get_attribute_data(const u8 *attr, u8 object_record_type, size_t buf_len, sc_cardctl_coolkey_attribute_t *attr_out)
{
	 
	if (object_record_type == COOLKEY_V0_OBJECT) {
		return coolkey_v0_get_attribute_data(attr, buf_len, attr_out);
	}

	 
	if (object_record_type != COOLKEY_V1_OBJECT) {
		return SC_ERROR_NO_CARD_SUPPORT;
	}

	return coolkey_v1_get_attribute_data(attr, buf_len, attr_out);

}
