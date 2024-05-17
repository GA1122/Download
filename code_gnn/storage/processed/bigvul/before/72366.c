static inline zend_long php_add_var_hash(php_serialize_data_t data, zval *var)  
{
	zval *zv;
	zend_ulong key;
	zend_bool is_ref = Z_ISREF_P(var);

	data->n += 1;

	if (!is_ref && Z_TYPE_P(var) != IS_OBJECT) {
		return 0;
	}

	 
	if (is_ref && Z_TYPE_P(Z_REFVAL_P(var)) == IS_OBJECT) {
		var = Z_REFVAL_P(var);
	}

	 
	key = (zend_ulong) (zend_uintptr_t) Z_COUNTED_P(var);
	zv = zend_hash_index_find(&data->ht, key);

	if (zv) {
		 
		if (is_ref) {
			data->n -= 1;
		}

		return Z_LVAL_P(zv);
	} else {
		zval zv_n;
		ZVAL_LONG(&zv_n, data->n);
		zend_hash_index_add_new(&data->ht, key, &zv_n);

		 
		zend_hash_index_add_new(&data->ht, key + 1, var);
		Z_ADDREF_P(var);

		return 0;
	}
}
 
