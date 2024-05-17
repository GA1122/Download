PHP_METHOD(HttpUrl, toArray)
{
	php_http_url_t *purl;

	if (SUCCESS != zend_parse_parameters_none()) {
		return;
	}

	 
	purl = php_http_url_from_struct(HASH_OF(getThis()));
	php_http_url_to_struct(purl, return_value TSRMLS_CC);
	php_http_url_free(&purl);
}