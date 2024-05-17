PHP_METHOD(HttpUrl, mod)
{
	zval *new_url = NULL;
	long flags = PHP_HTTP_URL_JOIN_PATH | PHP_HTTP_URL_JOIN_QUERY | PHP_HTTP_URL_SANITIZE_PATH;
	zend_error_handling zeh;

	php_http_expect(SUCCESS == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z!|l", &new_url, &flags), invalid_arg, return);

	zend_replace_error_handling(EH_THROW, php_http_exception_bad_url_class_entry, &zeh TSRMLS_CC);
	{
		php_http_url_t *new_purl = NULL, *old_purl = NULL;

		if (new_url) {
			new_purl = php_http_url_from_zval(new_url, flags TSRMLS_CC);
			if (!new_purl) {
				zend_restore_error_handling(&zeh TSRMLS_CC);
				return;
			}
		}

		if ((old_purl = php_http_url_from_struct(HASH_OF(getThis())))) {
			php_http_url_t *res_purl;

			ZVAL_OBJVAL(return_value, zend_objects_clone_obj(getThis() TSRMLS_CC), 0);

			res_purl = php_http_url_mod(old_purl, new_purl, flags TSRMLS_CC);
			php_http_url_to_struct(res_purl, return_value TSRMLS_CC);

			php_http_url_free(&res_purl);
			php_http_url_free(&old_purl);
		}
		if (new_purl) {
			php_http_url_free(&new_purl);
		}
	}
	zend_restore_error_handling(&zeh TSRMLS_CC);
}
