static PHP_MINFO_FUNCTION(zlib)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "ZLib Support", "enabled");
	php_info_print_table_row(2, "Stream Wrapper", "compress.zlib://");
	php_info_print_table_row(2, "Stream Filter", "zlib.inflate, zlib.deflate");
	php_info_print_table_row(2, "Compiled Version", ZLIB_VERSION);
	php_info_print_table_row(2, "Linked Version", (char *) zlibVersion());
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
