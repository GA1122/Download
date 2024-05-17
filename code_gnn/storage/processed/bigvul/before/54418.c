static int php_zip_add_file(struct zip *za, const char *filename, size_t filename_len,
	char *entry_name, size_t entry_name_len, long offset_start, long offset_len)  
{
	struct zip_source *zs;
	char resolved_path[MAXPATHLEN];
	zval exists_flag;


	if (ZIP_OPENBASEDIR_CHECKPATH(filename)) {
		return -1;
	}

	if (!expand_filepath(filename, resolved_path)) {
		return -1;
	}

	php_stat(resolved_path, strlen(resolved_path), FS_EXISTS, &exists_flag);
	if (Z_TYPE(exists_flag) == IS_FALSE) {
		return -1;
	}

	zs = zip_source_file(za, resolved_path, offset_start, offset_len);
	if (!zs) {
		return -1;
	}
	if (zip_file_add(za, entry_name, zs, ZIP_FL_OVERWRITE) < 0) {
		zip_source_free(zs);
		return -1;
	} else {
		zip_error_clear(za);
		return 1;
	}
}
 
