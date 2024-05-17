PHPAPI void var_destroy(php_unserialize_data_t *var_hashx)
{
	void *next;
	zend_long i;
	var_entries *var_hash = (*var_hashx)->first;
	var_dtor_entries *var_dtor_hash = (*var_hashx)->first_dtor;
	zend_bool wakeup_failed = 0;
	zval wakeup_name;
	ZVAL_UNDEF(&wakeup_name);

#if VAR_ENTRIES_DBG
	fprintf(stderr, "var_destroy(%ld)\n", var_hash?var_hash->used_slots:-1L);
#endif

	while (var_hash) {
		next = var_hash->next;
		efree_size(var_hash, sizeof(var_entries));
		var_hash = next;
	}

	while (var_dtor_hash) {
		for (i = 0; i < var_dtor_hash->used_slots; i++) {
			zval *zv = &var_dtor_hash->data[i];
#if VAR_ENTRIES_DBG
			fprintf(stderr, "var_destroy dtor(%p, %ld)\n", var_dtor_hash->data[i], Z_REFCOUNT_P(var_dtor_hash->data[i]));
#endif

			 
			if (Z_VAR_FLAGS_P(zv) == VAR_WAKEUP_FLAG) {
				if (!wakeup_failed) {
					zval retval;
					if (Z_ISUNDEF(wakeup_name)) {
						ZVAL_STRINGL(&wakeup_name, "__wakeup", sizeof("__wakeup") - 1);
					}

					BG(serialize_lock)++;
					if (call_user_function_ex(CG(function_table), zv, &wakeup_name, &retval, 0, 0, 1, NULL) == FAILURE || Z_ISUNDEF(retval)) {
						wakeup_failed = 1;
						GC_FLAGS(Z_OBJ_P(zv)) |= IS_OBJ_DESTRUCTOR_CALLED;
					}
					BG(serialize_lock)--;

					zval_ptr_dtor(&retval);
				} else {
					GC_FLAGS(Z_OBJ_P(zv)) |= IS_OBJ_DESTRUCTOR_CALLED;
				}
			}

			zval_ptr_dtor(zv);
		}
		next = var_dtor_hash->next;
		efree_size(var_dtor_hash, sizeof(var_dtor_entries));
		var_dtor_hash = next;
	}

	zval_ptr_dtor(&wakeup_name);
}