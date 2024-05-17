static PHP_FUNCTION(dbstmt_constructor)  
{
	php_error_docref(NULL TSRMLS_CC, E_ERROR, "You should not create a PDOStatement manually");
}
 
