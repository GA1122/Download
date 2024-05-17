static PHP_RINIT_FUNCTION(zlib)
{
	ZLIBG(compression_coding) = 0;
    if (!ZLIBG(handler_registered)) {
        ZLIBG(output_compression) = ZLIBG(output_compression_default);
        php_zlib_output_compression_start(TSRMLS_C);
    }

	return SUCCESS;
}
