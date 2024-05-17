static PHP_FUNCTION(zlib_get_coding_type)
{
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	switch (ZLIBG(compression_coding)) {
		case PHP_ZLIB_ENCODING_GZIP:
			RETURN_STRINGL("gzip", sizeof("gzip") - 1, 1);
		case PHP_ZLIB_ENCODING_DEFLATE:
			RETURN_STRINGL("deflate", sizeof("deflate") - 1, 1);
		default:
			RETURN_FALSE;
	}
}
