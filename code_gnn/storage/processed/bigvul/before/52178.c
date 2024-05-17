PHP_MSHUTDOWN_FUNCTION(file)  
{
#ifndef ZTS
	file_globals_dtor(&file_globals TSRMLS_CC);
#endif
	return SUCCESS;
}
 
