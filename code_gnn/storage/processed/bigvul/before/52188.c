PHPAPI int php_copy_file_ex(char *src, char *dest, int src_flg TSRMLS_DC)
{
	return php_copy_file_ctx(src, dest, 0, NULL TSRMLS_CC);
}
