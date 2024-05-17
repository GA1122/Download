coolkey_v0_get_attribute_record_len(const u8 *attr, size_t buf_len)
{
	size_t attribute_len = sizeof(coolkey_v0_attribute_header_t);
	size_t len;
	int r;

	r = coolkey_v0_get_attribute_len(attr, buf_len, &len);
	if (r < 0) {
		return buf_len;  
	}
	return MIN(buf_len,attribute_len+len);
}