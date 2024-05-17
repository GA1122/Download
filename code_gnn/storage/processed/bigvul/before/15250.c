PHP_FUNCTION(stream_socket_pair)
{
	long domain, type, protocol;
	php_stream *s1, *s2;
	php_socket_t pair[2];

	if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lll",
			&domain, &type, &protocol)) {
		RETURN_FALSE;
	}

	if (0 != socketpair(domain, type, protocol, pair)) {
		char errbuf[256];
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to create sockets: [%d]: %s",
			php_socket_errno(), php_socket_strerror(php_socket_errno(), errbuf, sizeof(errbuf)));
		RETURN_FALSE;
	}

	array_init(return_value);

	s1 = php_stream_sock_open_from_socket(pair[0], 0);
	s2 = php_stream_sock_open_from_socket(pair[1], 0);

	 
	php_stream_auto_cleanup(s1);
	php_stream_auto_cleanup(s2);

	add_next_index_resource(return_value, php_stream_get_resource_id(s1));
	add_next_index_resource(return_value, php_stream_get_resource_id(s2));
}