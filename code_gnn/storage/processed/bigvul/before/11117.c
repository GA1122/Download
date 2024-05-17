int phar_entry_delref(phar_entry_data *idata)  
{
	int ret = 0;

	if (idata->internal_file && !idata->internal_file->is_persistent) {
		if (--idata->internal_file->fp_refcount < 0) {
			idata->internal_file->fp_refcount = 0;
		}

		if (idata->fp && idata->fp != idata->phar->fp && idata->fp != idata->phar->ufp && idata->fp != idata->internal_file->fp) {
			php_stream_close(idata->fp);
		}
		 
		if (idata->internal_file->is_temp_dir) {
			destroy_phar_manifest_entry_int(idata->internal_file);
			efree(idata->internal_file);
		}
	}

	phar_archive_delref(idata->phar);
	efree(idata);
	return ret;
}
 