PHP_RSHUTDOWN_FUNCTION(pgsql)
{
	 
	zend_hash_clean(&PGG(notices));
	 
	zend_hash_apply(&EG(persistent_list), (apply_func_t) _rollback_transactions);
	return SUCCESS;
}
