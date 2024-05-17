PHP_MSHUTDOWN_FUNCTION(openssl)
{
	EVP_cleanup();

	 
	CRYPTO_set_locking_callback(NULL);
	 
	ERR_free_strings();

	php_unregister_url_stream_wrapper("https");
	php_unregister_url_stream_wrapper("ftps");

	php_stream_xport_unregister("ssl");
#ifndef OPENSSL_NO_SSL3
	php_stream_xport_unregister("sslv3");
#endif
	php_stream_xport_unregister("tls");
	php_stream_xport_unregister("tlsv1.0");
	php_stream_xport_unregister("tlsv1.1");
	php_stream_xport_unregister("tlsv1.2");

	 
	php_stream_xport_register("tcp", php_stream_generic_socket_factory);

	UNREGISTER_INI_ENTRIES();

	return SUCCESS;
}