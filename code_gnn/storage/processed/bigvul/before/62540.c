static inline int object_common2(UNSERIALIZE_PARAMETER, zend_long elements)
{
	HashTable *ht;
	zend_bool has_wakeup;

	if (Z_TYPE_P(rval) != IS_OBJECT) {
		return 0;
	}

	has_wakeup = Z_OBJCE_P(rval) != PHP_IC_ENTRY
		&& zend_hash_str_exists(&Z_OBJCE_P(rval)->function_table, "__wakeup", sizeof("__wakeup")-1);

	ht = Z_OBJPROP_P(rval);
	if (elements >= HT_MAX_SIZE - zend_hash_num_elements(ht)) {
		return 0;
	}

	zend_hash_extend(ht, zend_hash_num_elements(ht) + elements, (ht->u.flags & HASH_FLAG_PACKED));
	if (!process_nested_data(UNSERIALIZE_PASSTHRU, ht, elements, 1)) {
		if (has_wakeup) {
			ZVAL_DEREF(rval);
			GC_FLAGS(Z_OBJ_P(rval)) |= IS_OBJ_DESTRUCTOR_CALLED;
		}
		return 0;
	}

	ZVAL_DEREF(rval);
	if (has_wakeup) {
		 
		zval *wakeup_var = var_tmp_var(var_hash);
		ZVAL_COPY(wakeup_var, rval);
		Z_VAR_FLAGS_P(wakeup_var) = VAR_WAKEUP_FLAG;
	}

	return finish_nested_data(UNSERIALIZE_PASSTHRU);
}