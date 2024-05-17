PHP_METHOD(HttpUrl, toString)
{
	if (SUCCESS == zend_parse_parameters_none()) {
		php_http_url_t *purl;

		if ((purl = php_http_url_from_struct(HASH_OF(getThis())))) {
			char *str;
			size_t len;

			php_http_url_to_string(purl, &str, &len, 0);
			php_http_url_free(&purl);
			RETURN_STRINGL(str, len, 0);
		}
	}
	RETURN_EMPTY_STRING();
}
