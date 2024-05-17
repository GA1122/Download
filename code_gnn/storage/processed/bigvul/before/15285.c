static int parse_context_options(php_stream_context *context, zval *options TSRMLS_DC)
{
	HashPosition pos, opos;
	zval **wval, **oval;
	char *wkey, *okey;
	uint wkey_len, okey_len;
	int ret = SUCCESS;
	ulong num_key;

	zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(options), &pos);
	while (SUCCESS == zend_hash_get_current_data_ex(Z_ARRVAL_P(options), (void**)&wval, &pos)) {
		if (HASH_KEY_IS_STRING == zend_hash_get_current_key_ex(Z_ARRVAL_P(options), &wkey, &wkey_len, &num_key, 0, &pos)
				&& Z_TYPE_PP(wval) == IS_ARRAY) {

			zend_hash_internal_pointer_reset_ex(Z_ARRVAL_PP(wval), &opos);
			while (SUCCESS == zend_hash_get_current_data_ex(Z_ARRVAL_PP(wval), (void**)&oval, &opos)) {

				if (HASH_KEY_IS_STRING == zend_hash_get_current_key_ex(Z_ARRVAL_PP(wval), &okey, &okey_len, &num_key, 0, &opos)) {
					php_stream_context_set_option(context, wkey, okey, *oval);
				}
				zend_hash_move_forward_ex(Z_ARRVAL_PP(wval), &opos);
			}

		} else {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "options should have the form [\"wrappername\"][\"optionname\"] = $value");
		}
		zend_hash_move_forward_ex(Z_ARRVAL_P(options), &pos);
	}

	return ret;
}
