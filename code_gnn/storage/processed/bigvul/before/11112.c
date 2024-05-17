static zend_op_array *phar_compile_file(zend_file_handle *file_handle, int type)  
{
	zend_op_array *res;
	char *name = NULL;
	int failed;
	phar_archive_data *phar;

	if (!file_handle || !file_handle->filename) {
		return phar_orig_compile_file(file_handle, type);
	}
	if (strstr(file_handle->filename, ".phar") && !strstr(file_handle->filename, "://")) {
		if (SUCCESS == phar_open_from_filename((char*)file_handle->filename, strlen(file_handle->filename), NULL, 0, 0, &phar, NULL)) {
			if (phar->is_zip || phar->is_tar) {
				zend_file_handle f = *file_handle;

				 
				spprintf(&name, 4096, "phar://%s/%s", file_handle->filename, ".phar/stub.php");
				if (SUCCESS == phar_orig_zend_open((const char *)name, file_handle)) {
					efree(name);
					name = NULL;
					file_handle->filename = f.filename;
					if (file_handle->opened_path) {
						efree(file_handle->opened_path);
					}
					file_handle->opened_path = f.opened_path;
					file_handle->free_filename = f.free_filename;
				} else {
					*file_handle = f;
				}
			} else if (phar->flags & PHAR_FILE_COMPRESSION_MASK) {
				 
				file_handle->type = ZEND_HANDLE_STREAM;
				 
				file_handle->handle.stream.handle  = phar;
				file_handle->handle.stream.reader  = phar_zend_stream_reader;
				file_handle->handle.stream.closer  = NULL;
				file_handle->handle.stream.fsizer  = phar_zend_stream_fsizer;
				file_handle->handle.stream.isatty  = 0;
				phar->is_persistent ?
					php_stream_rewind(PHAR_G(cached_fp)[phar->phar_pos].fp) :
					php_stream_rewind(phar->fp);
				memset(&file_handle->handle.stream.mmap, 0, sizeof(file_handle->handle.stream.mmap));
			}
		}
	}

	zend_try {
		failed = 0;
		CG(zend_lineno) = 0;
		res = phar_orig_compile_file(file_handle, type);
	} zend_catch {
		failed = 1;
		res = NULL;
	} zend_end_try();

	if (name) {
		efree(name);
	}

	if (failed) {
		zend_bailout();
	}

	return res;
}
 