static int exif_process_string(char **result, char *value, size_t byte_count TSRMLS_DC) {
	 
	if ((byte_count=php_strnlen(value, byte_count)) > 0) {
		return exif_process_undefined(result, value, byte_count TSRMLS_CC);
	}
	(*result) = estrndup("", 1);  
	return byte_count+1;
}
