static void php_zip_add_from_pattern(INTERNAL_FUNCTION_PARAMETERS, int type)  
{
	struct zip *intern;
	zval *self = getThis();
	char *path = NULL;
	char *remove_path = NULL;
	char *add_path = NULL;
	size_t  add_path_len, remove_path_len = 0, path_len = 0;
	zend_long remove_all_path = 0;
	zend_long flags = 0;
	zval *options = NULL;
	int found;
	zend_string *pattern;

	if (!self) {
		RETURN_FALSE;
	}

	ZIP_FROM_OBJECT(intern, self);
	 
	if (type == 1) {
		if (zend_parse_parameters(ZEND_NUM_ARGS(), "P|la",
					&pattern, &flags, &options) == FAILURE) {
			return;
		}
	} else {
		if (zend_parse_parameters(ZEND_NUM_ARGS(), "P|sa",
					&pattern, &path, &path_len, &options) == FAILURE) {
			return;
		}
	}

	if (ZSTR_LEN(pattern) == 0) {
		php_error_docref(NULL, E_NOTICE, "Empty string as pattern");
		RETURN_FALSE;
	}
	if (options && (php_zip_parse_options(options, &remove_all_path, &remove_path, &remove_path_len,
			&add_path, &add_path_len) < 0)) {
		RETURN_FALSE;
	}

	if (remove_path && remove_path_len > 1) {
		size_t real_len = strlen(remove_path);
		if ((real_len > 1) && ((remove_path[real_len - 1] == '/') || (remove_path[real_len - 1] == '\\'))) {
			remove_path[real_len - 1] = '\0';
		}
	}

	if (type == 1) {
		found = php_zip_glob(ZSTR_VAL(pattern), ZSTR_LEN(pattern), flags, return_value);
	} else {
		found = php_zip_pcre(pattern, path, path_len, return_value);
	}

	if (found > 0) {
		int i;
		zval *zval_file;

		for (i = 0; i < found; i++) {
			char *file_stripped, *entry_name;
			size_t entry_name_len, file_stripped_len;
			char entry_name_buf[MAXPATHLEN];
			zend_string *basename = NULL;

			if ((zval_file = zend_hash_index_find(Z_ARRVAL_P(return_value), i)) != NULL) {
				if (remove_all_path) {
					basename = php_basename(Z_STRVAL_P(zval_file), Z_STRLEN_P(zval_file), NULL, 0);
					file_stripped = ZSTR_VAL(basename);
					file_stripped_len = ZSTR_LEN(basename);
				} else if (remove_path && strstr(Z_STRVAL_P(zval_file), remove_path) != NULL) {
					file_stripped = Z_STRVAL_P(zval_file) + remove_path_len + 1;
					file_stripped_len = Z_STRLEN_P(zval_file) - remove_path_len - 1;
				} else {
					file_stripped = Z_STRVAL_P(zval_file);
					file_stripped_len = Z_STRLEN_P(zval_file);
				}

				if (add_path) {
					if ((add_path_len + file_stripped_len) > MAXPATHLEN) {
						php_error_docref(NULL, E_WARNING, "Entry name too long (max: %d, %pd given)",
						MAXPATHLEN - 1, (add_path_len + file_stripped_len));
						zval_ptr_dtor(return_value);
						RETURN_FALSE;
					}

					snprintf(entry_name_buf, MAXPATHLEN, "%s%s", add_path, file_stripped);
					entry_name = entry_name_buf;
					entry_name_len = strlen(entry_name);
				} else {
					entry_name = Z_STRVAL_P(zval_file);
					entry_name_len = Z_STRLEN_P(zval_file);
				}
				if (basename) {
					zend_string_release(basename);
					basename = NULL;
				}
				if (php_zip_add_file(intern, Z_STRVAL_P(zval_file), Z_STRLEN_P(zval_file),
					entry_name, entry_name_len, 0, 0) < 0) {
					zval_dtor(return_value);
					RETURN_FALSE;
				}
			}
		}
	}
}
 
