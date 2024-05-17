phar_entry_info * phar_open_jit(phar_archive_data *phar, phar_entry_info *entry, char **error TSRMLS_DC)  
{
	if (error) {
		*error = NULL;
	}
	 
	if (FAILURE == phar_open_entry_fp(entry, error, 1 TSRMLS_CC)) {
		return NULL;
	}
	if (-1 == phar_seek_efp(entry, 0, SEEK_SET, 0, 1 TSRMLS_CC)) {
		spprintf(error, 4096, "phar error: cannot seek to start of file \"%s\" in phar \"%s\"", entry->filename, phar->fname);
		return NULL;
	}
	return entry;
}
 