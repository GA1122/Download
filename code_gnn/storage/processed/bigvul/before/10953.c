static int exif_process_string_raw(char **result, char *value, size_t byte_count) {
	 
	if (byte_count) {
		(*result) = safe_emalloc(byte_count, 1, 1);
		memcpy(*result, value, byte_count);
		(*result)[byte_count] = '\0';
		return byte_count+1;
	}
	return 0;
}
