static int phar_zip_applysignature(phar_archive_data *phar, struct _phar_zip_pass *pass,
				   smart_str *metadata TSRMLS_DC)  
{
	 
	if (!phar->is_data || phar->sig_flags) {
		int signature_length;
		char *signature, sigbuf[8];
		phar_entry_info entry = {0};
		php_stream *newfile;
		off_t tell, st;

		newfile = php_stream_fopen_tmpfile();
		if (newfile == NULL) {
			spprintf(pass->error, 0, "phar error: unable to create temporary file for the signature file");
			return FAILURE;
		}
		st = tell = php_stream_tell(pass->filefp);
		 
		php_stream_seek(pass->filefp, 0, SEEK_SET);
		phar_stream_copy_to_stream(pass->filefp, newfile, tell, NULL);
		tell = php_stream_tell(pass->centralfp);
		php_stream_seek(pass->centralfp, 0, SEEK_SET);
		phar_stream_copy_to_stream(pass->centralfp, newfile, tell, NULL);
		if (metadata->c) {
			php_stream_write(newfile, metadata->c, metadata->len);
		}

		if (FAILURE == phar_create_signature(phar, newfile, &signature, &signature_length, pass->error TSRMLS_CC)) {
			if (pass->error) {
				char *save = *(pass->error);
				spprintf(pass->error, 0, "phar error: unable to write signature to zip-based phar: %s", save);
				efree(save);
			}

			php_stream_close(newfile);
			return FAILURE;
		}

		entry.filename = ".phar/signature.bin";
		entry.filename_len = sizeof(".phar/signature.bin")-1;
		entry.fp = php_stream_fopen_tmpfile();
		entry.fp_type = PHAR_MOD;
		entry.is_modified = 1;
		if (entry.fp == NULL) {
			spprintf(pass->error, 0, "phar error: unable to create temporary file for signature");
			return FAILURE;
		}

		PHAR_SET_32(sigbuf, phar->sig_flags);
		PHAR_SET_32(sigbuf + 4, signature_length);

		if (8 != (int)php_stream_write(entry.fp, sigbuf, 8) || signature_length != (int)php_stream_write(entry.fp, signature, signature_length)) {
			efree(signature);
			if (pass->error) {
				spprintf(pass->error, 0, "phar error: unable to write signature to zip-based phar %s", phar->fname);
			}

			php_stream_close(newfile);
			return FAILURE;
		}

		efree(signature);
		entry.uncompressed_filesize = entry.compressed_filesize = signature_length + 8;
		entry.phar = phar;
		 
		phar_zip_changed_apply((void *)&entry, (void *)pass TSRMLS_CC);
		php_stream_close(newfile);

		if (pass->error && *(pass->error)) {
			 
			php_stream_close(newfile);
			return FAILURE;
		}
	}  
	return SUCCESS;
}
 