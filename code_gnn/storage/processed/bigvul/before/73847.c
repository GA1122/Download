HashTable *php_http_url_to_struct(const php_http_url_t *url, zval *strct TSRMLS_DC)
{
	zval arr;

	if (strct) {
		switch (Z_TYPE_P(strct)) {
			default:
				zval_dtor(strct);
				array_init(strct);
				 
			case IS_ARRAY:
			case IS_OBJECT:
				INIT_PZVAL_ARRAY((&arr), HASH_OF(strct));
				break;
		}
	} else {
		INIT_PZVAL(&arr);
		array_init(&arr);
	}

	if (url) {
		if (url->scheme) {
			add_assoc_string(&arr, "scheme", url->scheme, 1);
		}
		if (url->user) {
			add_assoc_string(&arr, "user", url->user, 1);
		}
		if (url->pass) {
			add_assoc_string(&arr, "pass", url->pass, 1);
		}
		if (url->host) {
			add_assoc_string(&arr, "host", url->host, 1);
		}
		if (url->port) {
			add_assoc_long(&arr, "port", (long) url->port);
		}
		if (url->path) {
			add_assoc_string(&arr, "path", url->path, 1);
		}
		if (url->query) {
			add_assoc_string(&arr, "query", url->query, 1);
		}
		if (url->fragment) {
			add_assoc_string(&arr, "fragment", url->fragment, 1);
		}
	}

	return Z_ARRVAL(arr);
}