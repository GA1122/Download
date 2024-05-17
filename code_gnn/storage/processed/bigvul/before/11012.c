static void php_str2num(bc_num *num, char *str TSRMLS_DC)
{
	char *p;

	if (!(p = strchr(str, '.'))) {
		bc_str2num(num, str, 0 TSRMLS_CC);
		return;
	}

	bc_str2num(num, str, strlen(p+1) TSRMLS_CC);
 }
