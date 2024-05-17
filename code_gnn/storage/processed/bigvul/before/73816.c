PHP_METHOD(HttpUrl, __construct)
{
	zval *new_url = NULL, *old_url = NULL;
	long flags = PHP_HTTP_URL_FROM_ENV;
	zend_error_handling zeh;

	php_http_expect(SUCCESS == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z!z!l", &old_url, &new_url, &flags), invalid_arg, return);

	zend_replace_error_handling(EH_THROW, php_http_exception_bad_url_class_entry, &zeh TSRMLS_CC);
	{
		php_http_url_t *res_purl, *new_purl = NULL, *old_purl = NULL;

		if (new_url) {
			new_purl = php_http_url_from_zval(new_url, flags TSRMLS_CC);
			if (!new_purl) {
				zend_restore_error_handling(&zeh TSRMLS_CC);
				return;
			}
		}
		if (old_url) {
			old_purl = php_http_url_from_zval(old_url, flags TSRMLS_CC);
			if (!old_purl) {
				if (new_purl) {
					php_http_url_free(&new_purl);
				}
				zend_restore_error_handling(&zeh TSRMLS_CC);
				return;
			}
		}

		res_purl = php_http_url_mod(old_purl, new_purl, flags TSRMLS_CC);
		php_http_url_to_struct(res_purl, getThis() TSRMLS_CC);

		php_http_url_free(&res_purl);
		if (old_purl) {
			php_http_url_free(&old_purl);
		}
		if (new_purl) {
			php_http_url_free(&new_purl);
		}
	}
	zend_restore_error_handling(&zeh TSRMLS_CC);
}
