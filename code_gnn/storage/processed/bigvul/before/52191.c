PHPAPI int php_fputcsv(php_stream *stream, zval *fields, char delimiter, char enclosure, char escape_char TSRMLS_DC)
{
	int count, i = 0, ret;
	zval **field_tmp = NULL, field;
	smart_str csvline = {0};
	HashPosition pos;

	count = zend_hash_num_elements(Z_ARRVAL_P(fields));
	zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(fields), &pos);
	while (zend_hash_get_current_data_ex(Z_ARRVAL_P(fields), (void **) &field_tmp, &pos) == SUCCESS) {
		field = **field_tmp;

		if (Z_TYPE_PP(field_tmp) != IS_STRING) {
			zval_copy_ctor(&field);
			convert_to_string(&field);
		}

		 
		if (FPUTCSV_FLD_CHK(delimiter) ||
			FPUTCSV_FLD_CHK(enclosure) ||
			FPUTCSV_FLD_CHK(escape_char) ||
			FPUTCSV_FLD_CHK('\n') ||
			FPUTCSV_FLD_CHK('\r') ||
			FPUTCSV_FLD_CHK('\t') ||
			FPUTCSV_FLD_CHK(' ')
		) {
			char *ch = Z_STRVAL(field);
			char *end = ch + Z_STRLEN(field);
			int escaped = 0;

			smart_str_appendc(&csvline, enclosure);
			while (ch < end) {
				if (*ch == escape_char) {
					escaped = 1;
				} else if (!escaped && *ch == enclosure) {
					smart_str_appendc(&csvline, enclosure);
				} else {
					escaped = 0;
				}
				smart_str_appendc(&csvline, *ch);
				ch++;
			}
			smart_str_appendc(&csvline, enclosure);
		} else {
			smart_str_appendl(&csvline, Z_STRVAL(field), Z_STRLEN(field));
		}

		if (++i != count) {
			smart_str_appendl(&csvline, &delimiter, 1);
		}
		zend_hash_move_forward_ex(Z_ARRVAL_P(fields), &pos);

		if (Z_TYPE_PP(field_tmp) != IS_STRING) {
			zval_dtor(&field);
		}
	}

	smart_str_appendc(&csvline, '\n');
	smart_str_0(&csvline);

	ret = php_stream_write(stream, csvline.c, csvline.len);

	smart_str_free(&csvline);

	return ret;
}
