int phar_open_or_create_filename(char *fname, int fname_len, char *alias, int alias_len, int is_data, int options, phar_archive_data** pphar, char **error)  
{
	const char *ext_str, *z;
	char *my_error;
	int ext_len;
	phar_archive_data **test, *unused = NULL;

	test = &unused;

	if (error) {
		*error = NULL;
	}

	 
	if (phar_detect_phar_fname_ext(fname, fname_len, &ext_str, &ext_len, !is_data, 0, 1) == SUCCESS) {
		goto check_file;
	}

	 
	if (FAILURE == phar_detect_phar_fname_ext(fname, fname_len, &ext_str, &ext_len, !is_data, 1, 1)) {
		if (error) {
			if (ext_len == -2) {
				spprintf(error, 0, "Cannot create a phar archive from a URL like \"%s\". Phar objects can only be created from local files", fname);
			} else {
				spprintf(error, 0, "Cannot create phar '%s', file extension (or combination) not recognised or the directory does not exist", fname);
			}
		}
		return FAILURE;
	}
check_file:
	if (phar_open_parsed_phar(fname, fname_len, alias, alias_len, is_data, options, test, &my_error) == SUCCESS) {
		if (pphar) {
			*pphar = *test;
		}

		if ((*test)->is_data && !(*test)->is_tar && !(*test)->is_zip) {
			if (error) {
				spprintf(error, 0, "Cannot open '%s' as a PharData object. Use Phar::__construct() for executable archives", fname);
			}
			return FAILURE;
		}

		if (PHAR_G(readonly) && !(*test)->is_data && ((*test)->is_tar || (*test)->is_zip)) {
			phar_entry_info *stub;
			if (NULL == (stub = zend_hash_str_find_ptr(&((*test)->manifest), ".phar/stub.php", sizeof(".phar/stub.php")-1))) {
				spprintf(error, 0, "'%s' is not a phar archive. Use PharData::__construct() for a standard zip or tar archive", fname);
				return FAILURE;
			}
		}

		if (!PHAR_G(readonly) || (*test)->is_data) {
			(*test)->is_writeable = 1;
		}
		return SUCCESS;
	} else if (my_error) {
		if (error) {
			*error = my_error;
		} else {
			efree(my_error);
		}
		return FAILURE;
	}

	if (ext_len > 3 && (z = memchr(ext_str, 'z', ext_len)) && ((ext_str + ext_len) - z >= 2) && !memcmp(z + 1, "ip", 2)) {
		 
		return phar_open_or_create_zip(fname, fname_len, alias, alias_len, is_data, options, pphar, error);
	}

	if (ext_len > 3 && (z = memchr(ext_str, 't', ext_len)) && ((ext_str + ext_len) - z >= 2) && !memcmp(z + 1, "ar", 2)) {
		 
		return phar_open_or_create_tar(fname, fname_len, alias, alias_len, is_data, options, pphar, error);
	}

	return phar_create_or_parse_filename(fname, fname_len, alias, alias_len, is_data, options, pphar, error);
}
 