zend_bool php_std_auto_global_callback(char *name, uint name_len)
{
	zend_printf("%s\n", name);
	return 0;  
}