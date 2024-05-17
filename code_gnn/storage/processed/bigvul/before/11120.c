int phar_flush(phar_archive_data *phar, char *user_stub, zend_long len, int convert, char **error)  
{
	char halt_stub[] = "__HALT_COMPILER();";
	zend_string *newstub;
	char *tmp;
	phar_entry_info *entry, *newentry;
	int halt_offset, restore_alias_len, global_flags = 0, closeoldfile;
	char *pos, has_dirs = 0;
	char manifest[18], entry_buffer[24];
	zend_off_t manifest_ftell;
	zend_long offset;
	size_t wrote;
	php_uint32 manifest_len, mytime, loc, new_manifest_count;
	php_uint32 newcrc32;
	php_stream *file, *oldfile, *newfile, *stubfile;
	php_stream_filter *filter;
	php_serialize_data_t metadata_hash;
	smart_str main_metadata_str = {0};
	int free_user_stub, free_fp = 1, free_ufp = 1;
	int manifest_hack = 0;

	if (phar->is_persistent) {
		if (error) {
			spprintf(error, 0, "internal error: attempt to flush cached zip-based phar \"%s\"", phar->fname);
		}
		return EOF;
	}

	if (error) {
		*error = NULL;
	}

	if (!zend_hash_num_elements(&phar->manifest) && !user_stub) {
		return EOF;
	}

	zend_hash_clean(&phar->virtual_dirs);

	if (phar->is_zip) {
		return phar_zip_flush(phar, user_stub, len, convert, error);
	}

	if (phar->is_tar) {
		return phar_tar_flush(phar, user_stub, len, convert, error);
	}

	if (PHAR_G(readonly)) {
		return EOF;
	}

	if (phar->fp && !phar->is_brandnew) {
		oldfile = phar->fp;
		closeoldfile = 0;
		php_stream_rewind(oldfile);
	} else {
		oldfile = php_stream_open_wrapper(phar->fname, "rb", 0, NULL);
		closeoldfile = oldfile != NULL;
	}
	newfile = php_stream_fopen_tmpfile();
	if (!newfile) {
		if (error) {
			spprintf(error, 0, "unable to create temporary file");
		}
		if (closeoldfile) {
			php_stream_close(oldfile);
		}
		return EOF;
	}

	if (user_stub) {
		zend_string *suser_stub;
		if (len < 0) {
			 
			if (!(php_stream_from_zval_no_verify(stubfile, (zval *)user_stub))) {
				if (closeoldfile) {
					php_stream_close(oldfile);
				}
				php_stream_close(newfile);
				if (error) {
					spprintf(error, 0, "unable to access resource to copy stub to new phar \"%s\"", phar->fname);
				}
				return EOF;
			}
			if (len == -1) {
				len = PHP_STREAM_COPY_ALL;
			} else {
				len = -len;
			}
			user_stub = 0;

			if (!(suser_stub = php_stream_copy_to_mem(stubfile, len, 0))) {
				if (closeoldfile) {
					php_stream_close(oldfile);
				}
				php_stream_close(newfile);
				if (error) {
					spprintf(error, 0, "unable to read resource to copy stub to new phar \"%s\"", phar->fname);
				}
				return EOF;
			}
			free_user_stub = 1;
			user_stub = ZSTR_VAL(suser_stub);
			len = ZSTR_LEN(suser_stub);
		} else {
			free_user_stub = 0;
		}
		tmp = estrndup(user_stub, len);
		if ((pos = php_stristr(tmp, halt_stub, len, sizeof(halt_stub) - 1)) == NULL) {
			efree(tmp);
			if (closeoldfile) {
				php_stream_close(oldfile);
			}
			php_stream_close(newfile);
			if (error) {
				spprintf(error, 0, "illegal stub for phar \"%s\"", phar->fname);
			}
			if (free_user_stub) {
				zend_string_free(suser_stub);
			}
			return EOF;
		}
		pos = user_stub + (pos - tmp);
		efree(tmp);
		len = pos - user_stub + 18;
		if ((size_t)len != php_stream_write(newfile, user_stub, len)
		||			  5 != php_stream_write(newfile, " ?>\r\n", 5)) {
			if (closeoldfile) {
				php_stream_close(oldfile);
			}
			php_stream_close(newfile);
			if (error) {
				spprintf(error, 0, "unable to create stub from string in new phar \"%s\"", phar->fname);
			}
			if (free_user_stub) {
				zend_string_free(suser_stub);
			}
			return EOF;
		}
		phar->halt_offset = len + 5;
		if (free_user_stub) {
			zend_string_free(suser_stub);
		}
	} else {
		size_t written;

		if (!user_stub && phar->halt_offset && oldfile && !phar->is_brandnew) {
			php_stream_copy_to_stream_ex(oldfile, newfile, phar->halt_offset, &written);
			newstub = NULL;
		} else {
			 
			newstub = phar_create_default_stub(NULL, NULL, NULL);
			phar->halt_offset = ZSTR_LEN(newstub);
			written = php_stream_write(newfile, ZSTR_VAL(newstub), phar->halt_offset);
		}
		if (phar->halt_offset != written) {
			if (closeoldfile) {
				php_stream_close(oldfile);
			}
			php_stream_close(newfile);
			if (error) {
				if (newstub) {
					spprintf(error, 0, "unable to create stub in new phar \"%s\"", phar->fname);
				} else {
					spprintf(error, 0, "unable to copy stub of old phar to new phar \"%s\"", phar->fname);
				}
			}
			if (newstub) {
				zend_string_free(newstub);
			}
			return EOF;
		}
		if (newstub) {
			zend_string_free(newstub);
		}
	}
	manifest_ftell = php_stream_tell(newfile);
	halt_offset = manifest_ftell;

	 
	zend_hash_apply(&phar->manifest, phar_flush_clean_deleted_apply);

	 
	main_metadata_str.s = NULL;
	if (Z_TYPE(phar->metadata) != IS_UNDEF) {
		PHP_VAR_SERIALIZE_INIT(metadata_hash);
		php_var_serialize(&main_metadata_str, &phar->metadata, &metadata_hash);
		PHP_VAR_SERIALIZE_DESTROY(metadata_hash);
	}
	new_manifest_count = 0;
	offset = 0;
	for (zend_hash_internal_pointer_reset(&phar->manifest);
		zend_hash_has_more_elements(&phar->manifest) == SUCCESS;
		zend_hash_move_forward(&phar->manifest)) {
		if ((entry = zend_hash_get_current_data_ptr(&phar->manifest)) == NULL) {
			continue;
		}
		if (entry->cfp) {
			 
			php_stream_close(entry->cfp);
			entry->cfp = 0;
		}
		if (entry->is_deleted || entry->is_mounted) {
			 
			continue;
		}
		if (!entry->is_modified && entry->fp_refcount) {
			 
			switch (entry->fp_type) {
				case PHAR_FP:
					free_fp = 0;
					break;
				case PHAR_UFP:
					free_ufp = 0;
				default:
					break;
			}
		}
		 
		++new_manifest_count;
		phar_add_virtual_dirs(phar, entry->filename, entry->filename_len);

		if (entry->is_dir) {
			 
			has_dirs = 1;
		}
		if (Z_TYPE(entry->metadata) != IS_UNDEF) {
			if (entry->metadata_str.s) {
				smart_str_free(&entry->metadata_str);
			}
			entry->metadata_str.s = NULL;
			PHP_VAR_SERIALIZE_INIT(metadata_hash);
			php_var_serialize(&entry->metadata_str, &entry->metadata, &metadata_hash);
			PHP_VAR_SERIALIZE_DESTROY(metadata_hash);
		} else {
			if (entry->metadata_str.s) {
				smart_str_free(&entry->metadata_str);
			}
			entry->metadata_str.s = NULL;
		}

		 
		offset += 4 + entry->filename_len + sizeof(entry_buffer) + (entry->metadata_str.s ? ZSTR_LEN(entry->metadata_str.s) : 0) + (entry->is_dir ? 1 : 0);

		 
		if ((oldfile && !entry->is_modified) || entry->is_dir) {
			if (entry->fp_type == PHAR_UFP) {
				 
				entry->fp_type = PHAR_FP;
			}
			continue;
		}
		if (!phar_get_efp(entry, 0)) {
			 
			newentry = phar_open_jit(phar, entry, error);
			if (!newentry) {
				 
				efree(*error);
				*error = NULL;
				continue;
			}
			entry = newentry;
		}
		file = phar_get_efp(entry, 0);
		if (-1 == phar_seek_efp(entry, 0, SEEK_SET, 0, 1)) {
			if (closeoldfile) {
				php_stream_close(oldfile);
			}
			php_stream_close(newfile);
			if (error) {
				spprintf(error, 0, "unable to seek to start of file \"%s\" while creating new phar \"%s\"", entry->filename, phar->fname);
			}
			return EOF;
		}
		newcrc32 = ~0;
		mytime = entry->uncompressed_filesize;
		for (loc = 0;loc < mytime; ++loc) {
			CRC32(newcrc32, php_stream_getc(file));
		}
		entry->crc32 = ~newcrc32;
		entry->is_crc_checked = 1;
		if (!(entry->flags & PHAR_ENT_COMPRESSION_MASK)) {
			 
			entry->compressed_filesize = entry->uncompressed_filesize;
			continue;
		}
		filter = php_stream_filter_create(phar_compress_filter(entry, 0), NULL, 0);
		if (!filter) {
			if (closeoldfile) {
				php_stream_close(oldfile);
			}
			php_stream_close(newfile);
			if (entry->flags & PHAR_ENT_COMPRESSED_GZ) {
				if (error) {
					spprintf(error, 0, "unable to gzip compress file \"%s\" to new phar \"%s\"", entry->filename, phar->fname);
				}
			} else {
				if (error) {
					spprintf(error, 0, "unable to bzip2 compress file \"%s\" to new phar \"%s\"", entry->filename, phar->fname);
				}
			}
			return EOF;
		}

		 
		 
		entry->cfp = php_stream_fopen_tmpfile();
		if (!entry->cfp) {
			if (error) {
				spprintf(error, 0, "unable to create temporary file");
			}
			if (closeoldfile) {
				php_stream_close(oldfile);
			}
			php_stream_close(newfile);
			return EOF;
		}
		php_stream_flush(file);
		if (-1 == phar_seek_efp(entry, 0, SEEK_SET, 0, 0)) {
			if (closeoldfile) {
				php_stream_close(oldfile);
			}
			php_stream_close(newfile);
			if (error) {
				spprintf(error, 0, "unable to seek to start of file \"%s\" while creating new phar \"%s\"", entry->filename, phar->fname);
			}
			return EOF;
		}
		php_stream_filter_append((&entry->cfp->writefilters), filter);
		if (SUCCESS != php_stream_copy_to_stream_ex(file, entry->cfp, entry->uncompressed_filesize, NULL)) {
			if (closeoldfile) {
				php_stream_close(oldfile);
			}
			php_stream_close(newfile);
			if (error) {
				spprintf(error, 0, "unable to copy compressed file contents of file \"%s\" while creating new phar \"%s\"", entry->filename, phar->fname);
			}
			return EOF;
		}
		php_stream_filter_flush(filter, 1);
		php_stream_flush(entry->cfp);
		php_stream_filter_remove(filter, 1);
		php_stream_seek(entry->cfp, 0, SEEK_END);
		entry->compressed_filesize = (php_uint32) php_stream_tell(entry->cfp);
		 
		php_stream_rewind(entry->cfp);
		entry->old_flags = entry->flags;
		entry->is_modified = 1;
		global_flags |= (entry->flags & PHAR_ENT_COMPRESSION_MASK);
	}
	global_flags |= PHAR_HDR_SIGNATURE;

	 
	 
	restore_alias_len = phar->alias_len;
	if (phar->is_temporary_alias) {
		phar->alias_len = 0;
	}

	manifest_len = offset + phar->alias_len + sizeof(manifest) + (main_metadata_str.s ? ZSTR_LEN(main_metadata_str.s) : 0);
	phar_set_32(manifest, manifest_len);
	 
	if(manifest[0] == '\r' || manifest[0] == '\n') {
		manifest_len++;
		phar_set_32(manifest, manifest_len);
		manifest_hack = 1;
	}
	phar_set_32(manifest+4, new_manifest_count);
	if (has_dirs) {
		*(manifest + 8) = (unsigned char) (((PHAR_API_VERSION) >> 8) & 0xFF);
		*(manifest + 9) = (unsigned char) (((PHAR_API_VERSION) & 0xF0));
	} else {
		*(manifest + 8) = (unsigned char) (((PHAR_API_VERSION_NODIR) >> 8) & 0xFF);
		*(manifest + 9) = (unsigned char) (((PHAR_API_VERSION_NODIR) & 0xF0));
	}
	phar_set_32(manifest+10, global_flags);
	phar_set_32(manifest+14, phar->alias_len);

	 
	if (sizeof(manifest) != php_stream_write(newfile, manifest, sizeof(manifest))
	|| (size_t)phar->alias_len != php_stream_write(newfile, phar->alias, phar->alias_len)) {

		if (closeoldfile) {
			php_stream_close(oldfile);
		}

		php_stream_close(newfile);
		phar->alias_len = restore_alias_len;

		if (error) {
			spprintf(error, 0, "unable to write manifest header of new phar \"%s\"", phar->fname);
		}

		return EOF;
	}

	phar->alias_len = restore_alias_len;

	phar_set_32(manifest, main_metadata_str.s ? ZSTR_LEN(main_metadata_str.s) : 0);
	if (4 != php_stream_write(newfile, manifest, 4) || ((main_metadata_str.s ? ZSTR_LEN(main_metadata_str.s) : 0)
	&& ZSTR_LEN(main_metadata_str.s) != php_stream_write(newfile, ZSTR_VAL(main_metadata_str.s), ZSTR_LEN(main_metadata_str.s)))) {
		smart_str_free(&main_metadata_str);

		if (closeoldfile) {
			php_stream_close(oldfile);
		}

		php_stream_close(newfile);
		phar->alias_len = restore_alias_len;

		if (error) {
			spprintf(error, 0, "unable to write manifest meta-data of new phar \"%s\"", phar->fname);
		}

		return EOF;
	}
	smart_str_free(&main_metadata_str);

	 
	manifest_ftell = php_stream_tell(newfile);

	 
	for (zend_hash_internal_pointer_reset(&phar->manifest);
		zend_hash_has_more_elements(&phar->manifest) == SUCCESS;
		zend_hash_move_forward(&phar->manifest)) {

		if ((entry = zend_hash_get_current_data_ptr(&phar->manifest)) == NULL) {
			continue;
		}

		if (entry->is_deleted || entry->is_mounted) {
			 
			continue;
		}

		if (entry->is_dir) {
			 
			phar_set_32(entry_buffer, entry->filename_len + 1);
		} else {
			phar_set_32(entry_buffer, entry->filename_len);
		}

		if (4 != php_stream_write(newfile, entry_buffer, 4)
		|| entry->filename_len != php_stream_write(newfile, entry->filename, entry->filename_len)
		|| (entry->is_dir && 1 != php_stream_write(newfile, "/", 1))) {
			if (closeoldfile) {
				php_stream_close(oldfile);
			}
			php_stream_close(newfile);
			if (error) {
				if (entry->is_dir) {
					spprintf(error, 0, "unable to write filename of directory \"%s\" to manifest of new phar \"%s\"", entry->filename, phar->fname);
				} else {
					spprintf(error, 0, "unable to write filename of file \"%s\" to manifest of new phar \"%s\"", entry->filename, phar->fname);
				}
			}
			return EOF;
		}

		 
		mytime = time(NULL);
		phar_set_32(entry_buffer, entry->uncompressed_filesize);
		phar_set_32(entry_buffer+4, mytime);
		phar_set_32(entry_buffer+8, entry->compressed_filesize);
		phar_set_32(entry_buffer+12, entry->crc32);
		phar_set_32(entry_buffer+16, entry->flags);
		phar_set_32(entry_buffer+20, entry->metadata_str.s ? ZSTR_LEN(entry->metadata_str.s) : 0);

		if (sizeof(entry_buffer) != php_stream_write(newfile, entry_buffer, sizeof(entry_buffer))
		|| (entry->metadata_str.s &&
		    ZSTR_LEN(entry->metadata_str.s) != php_stream_write(newfile, ZSTR_VAL(entry->metadata_str.s), ZSTR_LEN(entry->metadata_str.s)))) {
			if (closeoldfile) {
				php_stream_close(oldfile);
			}

			php_stream_close(newfile);

			if (error) {
				spprintf(error, 0, "unable to write temporary manifest of file \"%s\" to manifest of new phar \"%s\"", entry->filename, phar->fname);
			}

			return EOF;
		}
	}
	 
	if(manifest_hack) {
		if(1 != php_stream_write(newfile, manifest, 1)) {
			if (closeoldfile) {
				php_stream_close(oldfile);
			}

			php_stream_close(newfile);

			if (error) {
				spprintf(error, 0, "unable to write manifest padding byte");
			}

			return EOF;
		}
	}

	 
	offset = php_stream_tell(newfile);
	for (zend_hash_internal_pointer_reset(&phar->manifest);
		zend_hash_has_more_elements(&phar->manifest) == SUCCESS;
		zend_hash_move_forward(&phar->manifest)) {

		if ((entry = zend_hash_get_current_data_ptr(&phar->manifest)) == NULL) {
			continue;
		}

		if (entry->is_deleted || entry->is_dir || entry->is_mounted) {
			continue;
		}

		if (entry->cfp) {
			file = entry->cfp;
			php_stream_rewind(file);
		} else {
			file = phar_get_efp(entry, 0);
			if (-1 == phar_seek_efp(entry, 0, SEEK_SET, 0, 0)) {
				if (closeoldfile) {
					php_stream_close(oldfile);
				}
				php_stream_close(newfile);
				if (error) {
					spprintf(error, 0, "unable to seek to start of file \"%s\" while creating new phar \"%s\"", entry->filename, phar->fname);
				}
				return EOF;
			}
		}

		if (!file) {
			if (closeoldfile) {
				php_stream_close(oldfile);
			}
			php_stream_close(newfile);
			if (error) {
				spprintf(error, 0, "unable to seek to start of file \"%s\" while creating new phar \"%s\"", entry->filename, phar->fname);
			}
			return EOF;
		}

		 
		entry->offset = entry->offset_abs = offset;
		offset += entry->compressed_filesize;
		if (php_stream_copy_to_stream_ex(file, newfile, entry->compressed_filesize, &wrote) == FAILURE) {
			if (closeoldfile) {
				php_stream_close(oldfile);
			}

			php_stream_close(newfile);

			if (error) {
				spprintf(error, 0, "unable to write contents of file \"%s\" to new phar \"%s\"", entry->filename, phar->fname);
			}

			return EOF;
		}

		entry->is_modified = 0;

		if (entry->cfp) {
			php_stream_close(entry->cfp);
			entry->cfp = NULL;
		}

		if (entry->fp_type == PHAR_MOD) {
			 
			if (entry->fp_refcount == 0 && entry->fp != phar->fp && entry->fp != phar->ufp) {
				php_stream_close(entry->fp);
			}

			entry->fp = NULL;
			entry->fp_type = PHAR_FP;
		} else if (entry->fp_type == PHAR_UFP) {
			entry->fp_type = PHAR_FP;
		}
	}

	 
	if (global_flags & PHAR_HDR_SIGNATURE) {
		char sig_buf[4];

		php_stream_rewind(newfile);

		if (phar->signature) {
			efree(phar->signature);
			phar->signature = NULL;
		}

		switch(phar->sig_flags) {
#ifndef PHAR_HASH_OK
			case PHAR_SIG_SHA512:
			case PHAR_SIG_SHA256:
				if (closeoldfile) {
					php_stream_close(oldfile);
				}
				php_stream_close(newfile);
				if (error) {
					spprintf(error, 0, "unable to write contents of file \"%s\" to new phar \"%s\" with requested hash type", entry->filename, phar->fname);
				}
				return EOF;
#endif
			default: {
				char *digest = NULL;
				int digest_len;

				if (FAILURE == phar_create_signature(phar, newfile, &digest, &digest_len, error)) {
					if (error) {
						char *save = *error;
						spprintf(error, 0, "phar error: unable to write signature: %s", save);
						efree(save);
					}
					if (digest) {
						efree(digest);
					}
					if (closeoldfile) {
						php_stream_close(oldfile);
					}
					php_stream_close(newfile);
					return EOF;
				}

				php_stream_write(newfile, digest, digest_len);
				efree(digest);
				if (phar->sig_flags == PHAR_SIG_OPENSSL) {
					phar_set_32(sig_buf, digest_len);
					php_stream_write(newfile, sig_buf, 4);
				}
				break;
			}
		}
		phar_set_32(sig_buf, phar->sig_flags);
		php_stream_write(newfile, sig_buf, 4);
		php_stream_write(newfile, "GBMB", 4);
	}

	 
	if (phar->fp && free_fp) {
		php_stream_close(phar->fp);
	}

	if (phar->ufp) {
		if (free_ufp) {
			php_stream_close(phar->ufp);
		}
		phar->ufp = NULL;
	}

	if (closeoldfile) {
		php_stream_close(oldfile);
	}

	phar->internal_file_start = halt_offset + manifest_len + 4;
	phar->halt_offset = halt_offset;
	phar->is_brandnew = 0;

	php_stream_rewind(newfile);

	if (phar->donotflush) {
		 
		phar->fp = newfile;
	} else {
		phar->fp = php_stream_open_wrapper(phar->fname, "w+b", IGNORE_URL|STREAM_MUST_SEEK|REPORT_ERRORS, NULL);
		if (!phar->fp) {
			phar->fp = newfile;
			if (error) {
				spprintf(error, 4096, "unable to open new phar \"%s\" for writing", phar->fname);
			}
			return EOF;
		}

		if (phar->flags & PHAR_FILE_COMPRESSED_GZ) {
			 
			zval filterparams;

			array_init(&filterparams);
			add_assoc_long(&filterparams, "window", MAX_WBITS+16);
			filter = php_stream_filter_create("zlib.deflate", &filterparams, php_stream_is_persistent(phar->fp));
			zval_dtor(&filterparams);

			if (!filter) {
				if (error) {
					spprintf(error, 4096, "unable to compress all contents of phar \"%s\" using zlib, PHP versions older than 5.2.6 have a buggy zlib", phar->fname);
				}
				return EOF;
			}

			php_stream_filter_append(&phar->fp->writefilters, filter);
			php_stream_copy_to_stream_ex(newfile, phar->fp, PHP_STREAM_COPY_ALL, NULL);
			php_stream_filter_flush(filter, 1);
			php_stream_filter_remove(filter, 1);
			php_stream_close(phar->fp);
			 
			phar->fp = newfile;
		} else if (phar->flags & PHAR_FILE_COMPRESSED_BZ2) {
			filter = php_stream_filter_create("bzip2.compress", NULL, php_stream_is_persistent(phar->fp));
			php_stream_filter_append(&phar->fp->writefilters, filter);
			php_stream_copy_to_stream_ex(newfile, phar->fp, PHP_STREAM_COPY_ALL, NULL);
			php_stream_filter_flush(filter, 1);
			php_stream_filter_remove(filter, 1);
			php_stream_close(phar->fp);
			 
			phar->fp = newfile;
		} else {
			php_stream_copy_to_stream_ex(newfile, phar->fp, PHP_STREAM_COPY_ALL, NULL);
			 
			php_stream_close(newfile);
		}
	}

	if (-1 == php_stream_seek(phar->fp, phar->halt_offset, SEEK_SET)) {
		if (error) {
			spprintf(error, 0, "unable to seek to __HALT_COMPILER(); in new phar \"%s\"", phar->fname);
		}
		return EOF;
	}

	return EOF;
}
 
