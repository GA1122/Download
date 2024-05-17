static int zend_merge_property(zval **value TSRMLS_DC, int num_args, va_list args, const zend_hash_key *hash_key)  
{
	 
	if (hash_key->nKeyLength) {
		zval *obj = va_arg(args, zval *);
		zend_object_handlers *obj_ht = va_arg(args, zend_object_handlers *);
		zval *member;

		MAKE_STD_ZVAL(member);
		ZVAL_STRINGL(member, hash_key->arKey, hash_key->nKeyLength-1, 1);
		obj_ht->write_property(obj, member, *value, 0 TSRMLS_CC);
		zval_ptr_dtor(&member);
	}
	return ZEND_HASH_APPLY_KEEP;
}
 
