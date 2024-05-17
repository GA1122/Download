static int php_openssl_write_rand_file(const char * file, int egdsocket, int seeded)  
{
	char buffer[MAXPATHLEN];

	TSRMLS_FETCH();

	if (egdsocket || !seeded) {
		 
		return FAILURE;
	}
	if (file == NULL) {
		file = RAND_file_name(buffer, sizeof(buffer));
	}
	if (file == NULL || !RAND_write_file(file)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "unable to write random state");
		return FAILURE;
	}
	return SUCCESS;
}
 
