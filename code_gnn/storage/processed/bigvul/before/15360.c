static int php_zlib_output_conflict_check(const char *handler_name, size_t handler_name_len TSRMLS_DC)
{
	if (php_output_get_level(TSRMLS_C) > 0) {
		if (php_output_handler_conflict(handler_name, handler_name_len, ZEND_STRL(PHP_ZLIB_OUTPUT_HANDLER_NAME) TSRMLS_CC)
		||	php_output_handler_conflict(handler_name, handler_name_len, ZEND_STRL("ob_gzhandler") TSRMLS_CC)
		||  php_output_handler_conflict(handler_name, handler_name_len, ZEND_STRL("mb_output_handler") TSRMLS_CC)
		||	php_output_handler_conflict(handler_name, handler_name_len, ZEND_STRL("URL-Rewriter") TSRMLS_CC)) {
			return FAILURE;
		}
	}
	return SUCCESS;
}
