PHP_GSHUTDOWN_FUNCTION(phar)  
{
	zend_hash_destroy(&phar_globals->mime_types);
}
 