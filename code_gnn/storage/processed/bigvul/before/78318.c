coolkey_v0_get_attribute_count(const u8 *obj, size_t buf_len)
{
	coolkey_v0_object_header_t *object_head = (coolkey_v0_object_header_t *)obj;
	const u8 *attr;
	int count = 0;
	size_t attribute_data_len;

	 
	if (buf_len <= sizeof(coolkey_v0_object_header_t)) {
		return 0;
	}
	 
	attr = coolkey_attribute_start(obj, COOLKEY_V0_OBJECT, buf_len);
	if (attr == NULL) {
		return 0;
	}

	buf_len -= (attr-obj);
	attribute_data_len = bebytes2ushort(object_head->attribute_data_len);
	if (buf_len < attribute_data_len) {
		return 0;
	}

	while (attribute_data_len) {
		size_t len = coolkey_v0_get_attribute_record_len(attr, buf_len);

		if (len == 0) {
			break;
		}
		 
		if (len > attribute_data_len) {
			break;
		}
		 
		assert(len <= buf_len);
		count++;
		attr += len;
		buf_len -= len;
		attribute_data_len -= len;
	}
	return count;
}
