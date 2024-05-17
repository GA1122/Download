PHP_MINFO_FUNCTION(fileinfo)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "fileinfo support", "enabled");
	php_info_print_table_row(2, "version", PHP_FILEINFO_VERSION);
	php_info_print_table_end();
}
