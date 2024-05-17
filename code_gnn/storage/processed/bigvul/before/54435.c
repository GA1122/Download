static zval *php_zip_property_reader(ze_zip_object *obj, zip_prop_handler *hnd, zval *rv)  
{
	const char *retchar = NULL;
	int retint = 0;
	int len = 0;

	if (obj && obj->za != NULL) {
		if (hnd->read_const_char_func) {
			retchar = hnd->read_const_char_func(obj->za, &len);
		} else {
			if (hnd->read_int_func) {
				retint = hnd->read_int_func(obj->za);
				if (retint == -1) {
					php_error_docref(NULL, E_WARNING, "Internal zip error returned");
					return NULL;
				}
			} else {
				if (hnd->read_const_char_from_obj_func) {
					retchar = hnd->read_const_char_from_obj_func(obj);
					len = strlen(retchar);
				}
			}
		}
	}

	switch (hnd->type) {
		case IS_STRING:
			if (retchar) {
				ZVAL_STRINGL(rv, (char *) retchar, len);
			} else {
				ZVAL_EMPTY_STRING(rv);
			}
			break;
		 
		case IS_FALSE:
			ZVAL_BOOL(rv, (long)retint);
			break;
		case IS_LONG:
			ZVAL_LONG(rv, (long)retint);
			break;
		default:
			ZVAL_NULL(rv);
	}

	return rv;
}
 
