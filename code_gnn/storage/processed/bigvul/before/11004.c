static void xml_set_handler(zval **handler, zval **data)
{
	 
	if (*handler) {
		zval_ptr_dtor(handler);
	}

	 
	if (Z_TYPE_PP(data) != IS_ARRAY && Z_TYPE_PP(data) != IS_OBJECT) {

		convert_to_string_ex(data);
		if (Z_STRLEN_PP(data) == 0) {
			*handler = NULL;
			return;
		}
	}

	zval_add_ref(data);

	*handler = *data;
}
