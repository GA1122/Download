PHPAPI int php_mkdir(char *dir, long mode TSRMLS_DC)
{
	return php_mkdir_ex(dir, mode, REPORT_ERRORS TSRMLS_CC);
}
