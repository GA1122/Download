static int php_zlib_output_encoding(TSRMLS_D)
{
	zval **enc;

	if (!ZLIBG(compression_coding)) {
		zend_is_auto_global(ZEND_STRL("_SERVER") TSRMLS_CC);
		if (PG(http_globals)[TRACK_VARS_SERVER] && SUCCESS == zend_hash_find(Z_ARRVAL_P(PG(http_globals)[TRACK_VARS_SERVER]), "HTTP_ACCEPT_ENCODING", sizeof("HTTP_ACCEPT_ENCODING"), (void *) &enc)) {
			convert_to_string(*enc);
			if (strstr(Z_STRVAL_PP(enc), "gzip")) {
				ZLIBG(compression_coding) = PHP_ZLIB_ENCODING_GZIP;
			} else if (strstr(Z_STRVAL_PP(enc), "deflate")) {
				ZLIBG(compression_coding) = PHP_ZLIB_ENCODING_DEFLATE;
			}
		}
	}
	return ZLIBG(compression_coding);
}
