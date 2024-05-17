PHP_FUNCTION(mcrypt_get_cipher_name)
{
	char *cipher_dir_string;
	char *module_dir_string;
	char *cipher_name;
	char *cipher;
	int   cipher_len;
	MCRYPT td;

	MCRYPT_GET_INI

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
		&cipher, &cipher_len) == FAILURE) {
		return;
	}

	 
	td = mcrypt_module_open(cipher, cipher_dir_string, "ecb", module_dir_string);
	if (td != MCRYPT_FAILED) {
		cipher_name = mcrypt_enc_get_algorithms_name(td);
		mcrypt_module_close(td);
		RETVAL_STRING(cipher_name,1);
		mcrypt_free(cipher_name);
	} else {
		td = mcrypt_module_open(cipher, cipher_dir_string, "stream", module_dir_string);
		if (td != MCRYPT_FAILED) {
			cipher_name = mcrypt_enc_get_algorithms_name(td);
			mcrypt_module_close(td);
			RETVAL_STRING(cipher_name,1);
			mcrypt_free(cipher_name);
		} else {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, MCRYPT_OPEN_MODULE_FAILED);
			RETURN_FALSE;
		}
	}
}
