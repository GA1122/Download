PHPAPI int php_copy_file(char *src, char *dest TSRMLS_DC)
{
	return php_copy_file_ctx(src, dest, 0, NULL TSRMLS_CC);
}
