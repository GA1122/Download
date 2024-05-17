PHP_FUNCTION(mcrypt_module_close)
{
	MCRYPT_GET_TD_ARG
	zend_list_delete(Z_LVAL_P(mcryptind));
	RETURN_TRUE;
}
