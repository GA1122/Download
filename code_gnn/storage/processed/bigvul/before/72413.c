static int do_fetch(pdo_stmt_t *stmt, int do_bind, zval *return_value,
	enum pdo_fetch_type how, enum pdo_fetch_orientation ori, long offset, zval *return_all TSRMLS_DC)  
{
	int flags, idx, old_arg_count = 0;
	zend_class_entry *ce = NULL, *old_ce = NULL;
	zval grp_val, *grp, **pgrp, *retval, *old_ctor_args = NULL;
	int colno;

	if (how == PDO_FETCH_USE_DEFAULT) {
		how = stmt->default_fetch_type;
	}
	flags = how & PDO_FETCH_FLAGS;
	how = how & ~PDO_FETCH_FLAGS;

	if (!do_fetch_common(stmt, ori, offset, do_bind TSRMLS_CC)) {
		return 0;
	}

	if (how == PDO_FETCH_BOUND) {
		RETVAL_TRUE;
		return 1;
	}

	if (flags & PDO_FETCH_GROUP && stmt->fetch.column == -1) {
		colno = 1;
	} else {
		colno = stmt->fetch.column;
	}

	if (return_value) {
		int i = 0;

		if (how == PDO_FETCH_LAZY) {
			get_lazy_object(stmt, return_value TSRMLS_CC);
			return 1;
		}

		RETVAL_FALSE;

		switch (how) {
			case PDO_FETCH_USE_DEFAULT:
			case PDO_FETCH_ASSOC:
			case PDO_FETCH_BOTH:
			case PDO_FETCH_NUM:
			case PDO_FETCH_NAMED:
				if (!return_all) {
					ALLOC_HASHTABLE(return_value->value.ht);
					zend_hash_init(return_value->value.ht, stmt->column_count, NULL, ZVAL_PTR_DTOR, 0);
					Z_TYPE_P(return_value) = IS_ARRAY;
				} else {
					array_init(return_value);
				}
				break;

			case PDO_FETCH_KEY_PAIR:
				if (stmt->column_count != 2) {
					pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "PDO::FETCH_KEY_PAIR fetch mode requires the result set to contain extactly 2 columns." TSRMLS_CC);
					return 0;
				}
				if (!return_all) {
					array_init(return_value);
				}
				break;

			case PDO_FETCH_COLUMN:
				if (colno >= 0 && colno < stmt->column_count) {
					if (flags == PDO_FETCH_GROUP && stmt->fetch.column == -1) {
						fetch_value(stmt, return_value, 1, NULL TSRMLS_CC);
					} else if (flags == PDO_FETCH_GROUP && colno) {
						fetch_value(stmt, return_value, 0, NULL TSRMLS_CC);
					} else {
						fetch_value(stmt, return_value, colno, NULL TSRMLS_CC);
					}
					if (!return_all) {
						return 1;
					} else {
						break;
					}
				} else {
					pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "Invalid column index" TSRMLS_CC);
				}
				return 0;

			case PDO_FETCH_OBJ:
				object_init_ex(return_value, ZEND_STANDARD_CLASS_DEF_PTR);
				break;

			case PDO_FETCH_CLASS:
				if (flags & PDO_FETCH_CLASSTYPE) {
					zval val;
					zend_class_entry **cep;

					old_ce = stmt->fetch.cls.ce;
					old_ctor_args = stmt->fetch.cls.ctor_args;
					old_arg_count = stmt->fetch.cls.fci.param_count;
					do_fetch_opt_finish(stmt, 0 TSRMLS_CC);

					INIT_PZVAL(&val);
					fetch_value(stmt, &val, i++, NULL TSRMLS_CC);
					if (Z_TYPE(val) != IS_NULL) {
						convert_to_string(&val);
						if (zend_lookup_class(Z_STRVAL(val), Z_STRLEN(val), &cep TSRMLS_CC) == FAILURE) {
							stmt->fetch.cls.ce = ZEND_STANDARD_CLASS_DEF_PTR;
						} else {
							stmt->fetch.cls.ce = *cep;
						}
					}

					do_fetch_class_prepare(stmt TSRMLS_CC);
					zval_dtor(&val);
				}
				ce = stmt->fetch.cls.ce;
				if (!ce) {
					pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "No fetch class specified" TSRMLS_CC);
					return 0;
				}
				if ((flags & PDO_FETCH_SERIALIZE) == 0) {
					object_init_ex(return_value, ce);
					if (!stmt->fetch.cls.fci.size) {
						if (!do_fetch_class_prepare(stmt TSRMLS_CC))
						{
							return 0;
						}
					}
					if (ce->constructor && (flags & PDO_FETCH_PROPS_LATE)) {
						stmt->fetch.cls.fci.object_ptr = return_value;
						stmt->fetch.cls.fcc.object_ptr = return_value;
						if (zend_call_function(&stmt->fetch.cls.fci, &stmt->fetch.cls.fcc TSRMLS_CC) == FAILURE) {
							pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "could not call class constructor" TSRMLS_CC);
							return 0;
						} else {
							if (stmt->fetch.cls.retval_ptr) {
								zval_ptr_dtor(&stmt->fetch.cls.retval_ptr);
							}
						}
					}
				}
				break;

			case PDO_FETCH_INTO:
				if (!stmt->fetch.into) {
					pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "No fetch-into object specified." TSRMLS_CC);
					return 0;
					break;
				}

				Z_TYPE_P(return_value) = IS_OBJECT;
				Z_OBJ_HANDLE_P(return_value) = Z_OBJ_HANDLE_P(stmt->fetch.into);
				Z_OBJ_HT_P(return_value) = Z_OBJ_HT_P(stmt->fetch.into);
				zend_objects_store_add_ref(stmt->fetch.into TSRMLS_CC);

				if (zend_get_class_entry(return_value TSRMLS_CC) == ZEND_STANDARD_CLASS_DEF_PTR) {
					how = PDO_FETCH_OBJ;
				}
				break;

			case PDO_FETCH_FUNC:
				if (!stmt->fetch.func.function) {
					pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "No fetch function specified" TSRMLS_CC);
					return 0;
				}
				if (!stmt->fetch.func.fci.size) {
					if (!do_fetch_func_prepare(stmt TSRMLS_CC))
					{
						return 0;
					}
				}
				break;


			default:
				 
				return 0;
		}

		if (return_all && how != PDO_FETCH_KEY_PAIR) {
			INIT_PZVAL(&grp_val);
			if (flags == PDO_FETCH_GROUP && how == PDO_FETCH_COLUMN && stmt->fetch.column > 0) {
				fetch_value(stmt, &grp_val, colno, NULL TSRMLS_CC);
			} else {
				fetch_value(stmt, &grp_val, i, NULL TSRMLS_CC);
			}
			convert_to_string(&grp_val);
			if (how == PDO_FETCH_COLUMN) {
				i = stmt->column_count;  
			} else {
				i++;
			}
		}

		for (idx = 0; i < stmt->column_count; i++, idx++) {
			zval *val;
			MAKE_STD_ZVAL(val);
			fetch_value(stmt, val, i, NULL TSRMLS_CC);

			switch (how) {
				case PDO_FETCH_ASSOC:
					add_assoc_zval(return_value, stmt->columns[i].name, val);
					break;

				case PDO_FETCH_KEY_PAIR:
					{
						zval *tmp;
						MAKE_STD_ZVAL(tmp);
						fetch_value(stmt, tmp, ++i, NULL TSRMLS_CC);

						if (Z_TYPE_P(val) == IS_LONG) {
							zend_hash_index_update((return_all ? Z_ARRVAL_P(return_all) : Z_ARRVAL_P(return_value)), Z_LVAL_P(val), &tmp, sizeof(zval *), NULL);
						} else {
							convert_to_string(val);
							zend_symtable_update((return_all ? Z_ARRVAL_P(return_all) : Z_ARRVAL_P(return_value)), Z_STRVAL_P(val), Z_STRLEN_P(val) + 1, &tmp, sizeof(zval *), NULL);
						}
						zval_ptr_dtor(&val);
						return 1;
					}
					break;

				case PDO_FETCH_USE_DEFAULT:
				case PDO_FETCH_BOTH:
					add_assoc_zval(return_value, stmt->columns[i].name, val);
					Z_ADDREF_P(val);
					add_next_index_zval(return_value, val);
					break;

				case PDO_FETCH_NAMED:
					 
					{
						zval **curr_val = NULL;
						if (zend_hash_find(Z_ARRVAL_P(return_value), stmt->columns[i].name,
									strlen(stmt->columns[i].name)+1,
									(void**)&curr_val) == SUCCESS) {
							zval *arr;
							if (Z_TYPE_PP(curr_val) != IS_ARRAY) {
								 
								zval *cur;

								MAKE_STD_ZVAL(arr);
								array_init(arr);

								cur = *curr_val;
								*curr_val = arr;

								add_next_index_zval(arr, cur);
							} else {
								arr = *curr_val;
							}
							add_next_index_zval(arr, val);
						} else {
							add_assoc_zval(return_value, stmt->columns[i].name, val);
						}
					}
					break;

				case PDO_FETCH_NUM:
					add_next_index_zval(return_value, val);
					break;

				case PDO_FETCH_OBJ:
				case PDO_FETCH_INTO:
					zend_update_property(NULL, return_value,
						stmt->columns[i].name, stmt->columns[i].namelen,
						val TSRMLS_CC);
					zval_ptr_dtor(&val);
					break;

				case PDO_FETCH_CLASS:
					if ((flags & PDO_FETCH_SERIALIZE) == 0 || idx) {
						zend_update_property(ce, return_value,
							stmt->columns[i].name, stmt->columns[i].namelen,
							val TSRMLS_CC);
						zval_ptr_dtor(&val);
					} else {
#ifdef MBO_0
						php_unserialize_data_t var_hash;

						PHP_VAR_UNSERIALIZE_INIT(var_hash);
						if (php_var_unserialize(&return_value, (const unsigned char**)&Z_STRVAL_P(val), Z_STRVAL_P(val)+Z_STRLEN_P(val), NULL TSRMLS_CC) == FAILURE) {
							pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "cannot unserialize data" TSRMLS_CC);
							PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
							return 0;
						}
						PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
#endif
						if (!ce->unserialize) {
							zval_ptr_dtor(&val);
							pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "cannot unserialize class" TSRMLS_CC);
							return 0;
						} else if (ce->unserialize(&return_value, ce, (unsigned char *)(Z_TYPE_P(val) == IS_STRING ? Z_STRVAL_P(val) : ""), Z_TYPE_P(val) == IS_STRING ? Z_STRLEN_P(val) : 0, NULL TSRMLS_CC) == FAILURE) {
							zval_ptr_dtor(&val);
							pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "cannot unserialize class" TSRMLS_CC);
							zval_dtor(return_value);
							ZVAL_NULL(return_value);
							return 0;
						} else {
							zval_ptr_dtor(&val);
						}
					}
					break;

				case PDO_FETCH_FUNC:
					stmt->fetch.func.values[idx] = val;
					stmt->fetch.cls.fci.params[idx] = &stmt->fetch.func.values[idx];
					break;

				default:
					zval_ptr_dtor(&val);
					pdo_raise_impl_error(stmt->dbh, stmt, "22003", "mode is out of range" TSRMLS_CC);
					return 0;
					break;
			}
		}

		switch (how) {
			case PDO_FETCH_CLASS:
				if (ce->constructor && !(flags & (PDO_FETCH_PROPS_LATE | PDO_FETCH_SERIALIZE))) {
					stmt->fetch.cls.fci.object_ptr = return_value;
					stmt->fetch.cls.fcc.object_ptr = return_value;
					if (zend_call_function(&stmt->fetch.cls.fci, &stmt->fetch.cls.fcc TSRMLS_CC) == FAILURE) {
						pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "could not call class constructor" TSRMLS_CC);
						return 0;
					} else {
						if (stmt->fetch.cls.retval_ptr) {
							zval_ptr_dtor(&stmt->fetch.cls.retval_ptr);
						}
					}
				}
				if (flags & PDO_FETCH_CLASSTYPE) {
					do_fetch_opt_finish(stmt, 0 TSRMLS_CC);
					stmt->fetch.cls.ce = old_ce;
					stmt->fetch.cls.ctor_args = old_ctor_args;
					stmt->fetch.cls.fci.param_count = old_arg_count;
				}
				break;

			case PDO_FETCH_FUNC:
				stmt->fetch.func.fci.param_count = idx;
				stmt->fetch.func.fci.retval_ptr_ptr = &retval;
				if (zend_call_function(&stmt->fetch.func.fci, &stmt->fetch.func.fcc TSRMLS_CC) == FAILURE) {
					pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "could not call user-supplied function" TSRMLS_CC);
					return 0;
				} else {
					if (return_all) {
						zval_ptr_dtor(&return_value);  
						return_value = retval;
					} else if (retval) {
						MAKE_COPY_ZVAL(&retval, return_value);
						zval_ptr_dtor(&retval);
					}
				}
				while(idx--) {
					zval_ptr_dtor(&stmt->fetch.func.values[idx]);
				}
				break;

			default:
				break;
		}

		if (return_all) {
			if ((flags & PDO_FETCH_UNIQUE) == PDO_FETCH_UNIQUE) {
				add_assoc_zval(return_all, Z_STRVAL(grp_val), return_value);
			} else {
				if (zend_symtable_find(Z_ARRVAL_P(return_all), Z_STRVAL(grp_val), Z_STRLEN(grp_val)+1, (void**)&pgrp) == FAILURE) {
					MAKE_STD_ZVAL(grp);
					array_init(grp);
					add_assoc_zval(return_all, Z_STRVAL(grp_val), grp);
				} else {
					grp = *pgrp;
				}
				add_next_index_zval(grp, return_value);
			}
			zval_dtor(&grp_val);
		}

	}

	return 1;
}
 