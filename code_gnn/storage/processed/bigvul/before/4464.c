void phar_entry_remove(phar_entry_data *idata, char **error TSRMLS_DC)  
{
	phar_archive_data *phar;

	phar = idata->phar;

	if (idata->internal_file->fp_refcount < 2) {
		if (idata->fp && idata->fp != idata->phar->fp && idata->fp != idata->phar->ufp && idata->fp != idata->internal_file->fp) {
			php_stream_close(idata->fp);
		}
		zend_hash_del(&idata->phar->manifest, idata->internal_file->filename, idata->internal_file->filename_len);
		idata->phar->refcount--;
		efree(idata);
	} else {
		idata->internal_file->is_deleted = 1;
		phar_entry_delref(idata TSRMLS_CC);
	}

	if (!phar->donotflush) {
		phar_flush(phar, 0, 0, 0, error TSRMLS_CC);
	}
}
 