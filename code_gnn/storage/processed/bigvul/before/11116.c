int phar_detect_phar_fname_ext(const char *filename, int filename_len, const char **ext_str, int *ext_len, int executable, int for_create, int is_complete)  
{
	const char *pos, *slash;

	*ext_str = NULL;
	*ext_len = 0;

	if (!filename_len || filename_len == 1) {
		return FAILURE;
	}

	phar_request_initialize();
	 
	pos = memchr(filename, '/', filename_len);

	if (pos && pos != filename) {
		 
		if (*(pos - 1) == ':' && (pos - filename) < filename_len - 1 && *(pos + 1) == '/') {
			*ext_len = -2;
			*ext_str = NULL;
			return FAILURE;
		}
		if (zend_hash_str_exists(&(PHAR_G(phar_alias_map)), (char *) filename, pos - filename)) {
			*ext_str = pos;
			*ext_len = -1;
			return FAILURE;
		}

		if (PHAR_G(manifest_cached) && zend_hash_str_exists(&cached_alias, (char *) filename, pos - filename)) {
			*ext_str = pos;
			*ext_len = -1;
			return FAILURE;
		}
	}

	if (zend_hash_num_elements(&(PHAR_G(phar_fname_map))) || PHAR_G(manifest_cached)) {
		phar_archive_data *pphar;

		if (is_complete) {
			if (NULL != (pphar = zend_hash_str_find_ptr(&(PHAR_G(phar_fname_map)), (char *) filename, filename_len))) {
				*ext_str = filename + (filename_len - pphar->ext_len);
woohoo:
				*ext_len = pphar->ext_len;

				if (executable == 2) {
					return SUCCESS;
				}

				if (executable == 1 && !pphar->is_data) {
					return SUCCESS;
				}

				if (!executable && pphar->is_data) {
					return SUCCESS;
				}

				return FAILURE;
			}

			if (PHAR_G(manifest_cached) && NULL != (pphar = zend_hash_str_find_ptr(&cached_phars, (char *) filename, filename_len))) {
				*ext_str = filename + (filename_len - pphar->ext_len);
				goto woohoo;
			}
		} else {
			zend_string *str_key;
			zend_ulong unused;

			for (zend_hash_internal_pointer_reset(&(PHAR_G(phar_fname_map)));
				HASH_KEY_NON_EXISTENT != zend_hash_get_current_key(&(PHAR_G(phar_fname_map)), &str_key, &unused);
				zend_hash_move_forward(&(PHAR_G(phar_fname_map)))
			) {
				if (ZSTR_LEN(str_key) > (uint) filename_len) {
					continue;
				}

				if (!memcmp(filename, ZSTR_VAL(str_key), ZSTR_LEN(str_key)) && ((uint)filename_len == ZSTR_LEN(str_key)
					|| filename[ZSTR_LEN(str_key)] == '/' || filename[ZSTR_LEN(str_key)] == '\0')) {
					if (NULL == (pphar = zend_hash_get_current_data_ptr(&(PHAR_G(phar_fname_map))))) {
						break;
					}
					*ext_str = filename + (ZSTR_LEN(str_key) - pphar->ext_len);
					goto woohoo;
				}
			}

			if (PHAR_G(manifest_cached)) {
				for (zend_hash_internal_pointer_reset(&cached_phars);
					HASH_KEY_NON_EXISTENT != zend_hash_get_current_key(&cached_phars, &str_key, &unused);
					zend_hash_move_forward(&cached_phars)
				) {
					if (ZSTR_LEN(str_key) > (uint) filename_len) {
						continue;
					}

					if (!memcmp(filename, ZSTR_VAL(str_key), ZSTR_LEN(str_key)) && ((uint)filename_len == ZSTR_LEN(str_key)
						|| filename[ZSTR_LEN(str_key)] == '/' || filename[ZSTR_LEN(str_key)] == '\0')) {
						if (NULL == (pphar = zend_hash_get_current_data_ptr(&cached_phars))) {
							break;
						}
						*ext_str = filename + (ZSTR_LEN(str_key) - pphar->ext_len);
						goto woohoo;
					}
				}
			}
		}
	}

	pos = memchr(filename + 1, '.', filename_len);
next_extension:
	if (!pos) {
		return FAILURE;
	}

	while (pos != filename && (*(pos - 1) == '/' || *(pos - 1) == '\0')) {
		pos = memchr(pos + 1, '.', filename_len - (pos - filename) + 1);
		if (!pos) {
			return FAILURE;
		}
	}

	slash = memchr(pos, '/', filename_len - (pos - filename));

	if (!slash) {
		 
		*ext_str = pos;
		*ext_len = strlen(pos);

		 
		switch (phar_check_str(filename, *ext_str, *ext_len, executable, for_create)) {
			case SUCCESS:
				return SUCCESS;
			case FAILURE:
				 
				return FAILURE;
		}
	}

	 
	*ext_str = pos;
	*ext_len = slash - pos;

	switch (phar_check_str(filename, *ext_str, *ext_len, executable, for_create)) {
		case SUCCESS:
			return SUCCESS;
		case FAILURE:
			 
			pos = strchr(pos + 1, '.');
			if (pos) {
				*ext_str = NULL;
				*ext_len = 0;
			}
			goto next_extension;
	}

	return FAILURE;
}
 