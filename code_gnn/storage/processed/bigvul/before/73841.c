static php_http_url_t *php_http_url_from_env(TSRMLS_D)
{
	zval *https, *zhost, *zport;
	long port;
	php_http_buffer_t buf;

	php_http_buffer_init_ex(&buf, MAX(PHP_HTTP_BUFFER_DEFAULT_SIZE, sizeof(php_http_url_t)<<2), PHP_HTTP_BUFFER_INIT_PREALLOC);
	php_http_buffer_account(&buf, sizeof(php_http_url_t));
	memset(buf.data, 0, buf.used);

	 
	url(buf)->scheme = &buf.data[buf.used];
	https = php_http_env_get_server_var(ZEND_STRL("HTTPS"), 1 TSRMLS_CC);
	if (https && !strcasecmp(Z_STRVAL_P(https), "ON")) {
		php_http_buffer_append(&buf, "https", sizeof("https"));
	} else {
		php_http_buffer_append(&buf, "http", sizeof("http"));
	}

	 
	url(buf)->host = &buf.data[buf.used];
	if ((((zhost = php_http_env_get_server_var(ZEND_STRL("HTTP_HOST"), 1 TSRMLS_CC)) ||
			(zhost = php_http_env_get_server_var(ZEND_STRL("SERVER_NAME"), 1 TSRMLS_CC)) ||
			(zhost = php_http_env_get_server_var(ZEND_STRL("SERVER_ADDR"), 1 TSRMLS_CC)))) && Z_STRLEN_P(zhost)) {
		size_t stop_at = strspn(Z_STRVAL_P(zhost), "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-.");

		php_http_buffer_append(&buf, Z_STRVAL_P(zhost), stop_at);
		php_http_buffer_append(&buf, "", 1);
	} else {
		char *host_str = localhostname();

		php_http_buffer_append(&buf, host_str, strlen(host_str) + 1);
		efree(host_str);
	}

	 
	zport = php_http_env_get_server_var(ZEND_STRL("SERVER_PORT"), 1 TSRMLS_CC);
	if (zport && IS_LONG == is_numeric_string(Z_STRVAL_P(zport), Z_STRLEN_P(zport), &port, NULL, 0)) {
		url(buf)->port = port;
	}

	 
	if (SG(request_info).request_uri && SG(request_info).request_uri[0]) {
		const char *q = strchr(SG(request_info).request_uri, '?');

		url(buf)->path = &buf.data[buf.used];

		if (q) {
			php_http_buffer_append(&buf, SG(request_info).request_uri, q - SG(request_info).request_uri);
			php_http_buffer_append(&buf, "", 1);
		} else {
			php_http_buffer_append(&buf, SG(request_info).request_uri, strlen(SG(request_info).request_uri) + 1);
		}
	}

	 
	if (SG(request_info).query_string && SG(request_info).query_string[0]) {
		url(buf)->query = &buf.data[buf.used];
		php_http_buffer_append(&buf, SG(request_info).query_string, strlen(SG(request_info).query_string) + 1);
	}

	return url(buf);
}