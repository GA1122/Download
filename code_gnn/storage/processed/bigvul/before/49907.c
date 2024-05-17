static int phar_zip_changed_apply_int(phar_entry_info *entry, void *arg)  
{
	phar_zip_file_header local;
	phar_zip_unix3 perms;
	phar_zip_central_dir_file central;
	struct _phar_zip_pass *p;
	php_uint32 newcrc32;
	zend_off_t offset;
	int not_really_modified = 0;
	p = (struct _phar_zip_pass*) arg;

	if (entry->is_mounted) {
		return ZEND_HASH_APPLY_KEEP;
	}

	if (entry->is_deleted) {
		if (entry->fp_refcount <= 0) {
			return ZEND_HASH_APPLY_REMOVE;
		} else {
			 
			return ZEND_HASH_APPLY_KEEP;
		}
	}

	phar_add_virtual_dirs(entry->phar, entry->filename, entry->filename_len);
	memset(&local, 0, sizeof(local));
	memset(&central, 0, sizeof(central));
	memset(&perms, 0, sizeof(perms));
	strncpy(local.signature, "PK\3\4", 4);
	strncpy(central.signature, "PK\1\2", 4);
	PHAR_SET_16(central.extra_len, sizeof(perms));
	PHAR_SET_16(local.extra_len, sizeof(perms));
	perms.tag[0] = 'n';
	perms.tag[1] = 'u';
	PHAR_SET_16(perms.size, sizeof(perms) - 4);
	PHAR_SET_16(perms.perms, entry->flags & PHAR_ENT_PERM_MASK);
	{
		php_uint32 crc = (php_uint32) ~0;
		CRC32(crc, perms.perms[0]);
		CRC32(crc, perms.perms[1]);
		PHAR_SET_32(perms.crc32, ~crc);
	}

	if (entry->flags & PHAR_ENT_COMPRESSED_GZ) {
		PHAR_SET_16(central.compressed, PHAR_ZIP_COMP_DEFLATE);
		PHAR_SET_16(local.compressed, PHAR_ZIP_COMP_DEFLATE);
	}

	if (entry->flags & PHAR_ENT_COMPRESSED_BZ2) {
		PHAR_SET_16(central.compressed, PHAR_ZIP_COMP_BZIP2);
		PHAR_SET_16(local.compressed, PHAR_ZIP_COMP_BZIP2);
	}

	 
	phar_zip_u2d_time(entry->timestamp, local.timestamp, local.datestamp);
	memcpy(central.timestamp, local.timestamp, sizeof(local.timestamp));
	memcpy(central.datestamp, local.datestamp, sizeof(local.datestamp));
	PHAR_SET_16(central.filename_len, entry->filename_len + (entry->is_dir ? 1 : 0));
	PHAR_SET_16(local.filename_len, entry->filename_len + (entry->is_dir ? 1 : 0));
	PHAR_SET_32(central.offset, php_stream_tell(p->filefp));

	 
	if (entry->is_modified) {
		php_uint32 loc;
		php_stream_filter *filter;
		php_stream *efp;

		if (entry->is_dir) {
			entry->is_modified = 0;
			if (entry->fp_type == PHAR_MOD && entry->fp != entry->phar->fp && entry->fp != entry->phar->ufp) {
				php_stream_close(entry->fp);
				entry->fp = NULL;
				entry->fp_type = PHAR_FP;
			}
			goto continue_dir;
		}

		if (FAILURE == phar_open_entry_fp(entry, p->error, 0)) {
			spprintf(p->error, 0, "unable to open file contents of file \"%s\" in zip-based phar \"%s\"", entry->filename, entry->phar->fname);
			return ZEND_HASH_APPLY_STOP;
		}

		 
		if (entry->flags & PHAR_ENT_COMPRESSION_MASK && (entry->old_flags == entry->flags || !entry->old_flags)) {
			not_really_modified = 1;
			goto is_compressed;
		}

		if (-1 == phar_seek_efp(entry, 0, SEEK_SET, 0, 0)) {
			spprintf(p->error, 0, "unable to seek to start of file \"%s\" to zip-based phar \"%s\"", entry->filename, entry->phar->fname);
			return ZEND_HASH_APPLY_STOP;
		}

		efp = phar_get_efp(entry, 0);
		newcrc32 = ~0;

		for (loc = 0;loc < entry->uncompressed_filesize; ++loc) {
			CRC32(newcrc32, php_stream_getc(efp));
		}

		entry->crc32 = ~newcrc32;
		PHAR_SET_32(central.uncompsize, entry->uncompressed_filesize);
		PHAR_SET_32(local.uncompsize, entry->uncompressed_filesize);

		if (!(entry->flags & PHAR_ENT_COMPRESSION_MASK)) {
			 
			entry->compressed_filesize = entry->uncompressed_filesize;
			PHAR_SET_32(central.compsize, entry->uncompressed_filesize);
			PHAR_SET_32(local.compsize, entry->uncompressed_filesize);
			goto not_compressed;
		}

		filter = php_stream_filter_create(phar_compress_filter(entry, 0), NULL, 0);

		if (!filter) {
			if (entry->flags & PHAR_ENT_COMPRESSED_GZ) {
				spprintf(p->error, 0, "unable to gzip compress file \"%s\" to zip-based phar \"%s\"", entry->filename, entry->phar->fname);
			} else {
				spprintf(p->error, 0, "unable to bzip2 compress file \"%s\" to zip-based phar \"%s\"", entry->filename, entry->phar->fname);
			}
			return ZEND_HASH_APPLY_STOP;
		}

		 
		 
		entry->cfp = php_stream_fopen_tmpfile();

		if (!entry->cfp) {
			spprintf(p->error, 0, "unable to create temporary file for file \"%s\" while creating zip-based phar \"%s\"", entry->filename, entry->phar->fname);
			return ZEND_HASH_APPLY_STOP;
		}

		php_stream_flush(efp);

		if (-1 == phar_seek_efp(entry, 0, SEEK_SET, 0, 0)) {
			spprintf(p->error, 0, "unable to seek to start of file \"%s\" to zip-based phar \"%s\"", entry->filename, entry->phar->fname);
			return ZEND_HASH_APPLY_STOP;
		}

		php_stream_filter_append((&entry->cfp->writefilters), filter);

		if (SUCCESS != php_stream_copy_to_stream_ex(efp, entry->cfp, entry->uncompressed_filesize, NULL)) {
			spprintf(p->error, 0, "unable to copy compressed file contents of file \"%s\" while creating new phar \"%s\"", entry->filename, entry->phar->fname);
			return ZEND_HASH_APPLY_STOP;
		}

		php_stream_filter_flush(filter, 1);
		php_stream_flush(entry->cfp);
		php_stream_filter_remove(filter, 1);
		php_stream_seek(entry->cfp, 0, SEEK_END);
		entry->compressed_filesize = (php_uint32) php_stream_tell(entry->cfp);
		PHAR_SET_32(central.compsize, entry->compressed_filesize);
		PHAR_SET_32(local.compsize, entry->compressed_filesize);
		 
		php_stream_rewind(entry->cfp);
		entry->old_flags = entry->flags;
		entry->is_modified = 1;
	} else {
is_compressed:
		PHAR_SET_32(central.uncompsize, entry->uncompressed_filesize);
		PHAR_SET_32(local.uncompsize, entry->uncompressed_filesize);
		PHAR_SET_32(central.compsize, entry->compressed_filesize);
		PHAR_SET_32(local.compsize, entry->compressed_filesize);
		if (p->old) {
			if (-1 == php_stream_seek(p->old, entry->offset_abs, SEEK_SET)) {
				spprintf(p->error, 0, "unable to seek to start of file \"%s\" while creating zip-based phar \"%s\"", entry->filename, entry->phar->fname);
				return ZEND_HASH_APPLY_STOP;
			}
		}
	}
not_compressed:
	PHAR_SET_32(central.crc32, entry->crc32);
	PHAR_SET_32(local.crc32, entry->crc32);
continue_dir:
	 
	if (Z_TYPE(entry->metadata) != IS_UNDEF) {
		php_serialize_data_t metadata_hash;

		if (entry->metadata_str.s) {
			smart_str_free(&entry->metadata_str);
		}
		entry->metadata_str.s = NULL;
		PHP_VAR_SERIALIZE_INIT(metadata_hash);
		php_var_serialize(&entry->metadata_str, &entry->metadata, &metadata_hash);
		PHP_VAR_SERIALIZE_DESTROY(metadata_hash);
		PHAR_SET_16(central.comment_len, ZSTR_LEN(entry->metadata_str.s));
	}

	entry->header_offset = php_stream_tell(p->filefp);
	offset = entry->header_offset + sizeof(local) + entry->filename_len + (entry->is_dir ? 1 : 0) + sizeof(perms);

	if (sizeof(local) != php_stream_write(p->filefp, (char *)&local, sizeof(local))) {
		spprintf(p->error, 0, "unable to write local file header of file \"%s\" to zip-based phar \"%s\"", entry->filename, entry->phar->fname);
		return ZEND_HASH_APPLY_STOP;
	}

	if (sizeof(central) != php_stream_write(p->centralfp, (char *)&central, sizeof(central))) {
		spprintf(p->error, 0, "unable to write central directory entry for file \"%s\" while creating zip-based phar \"%s\"", entry->filename, entry->phar->fname);
		return ZEND_HASH_APPLY_STOP;
	}

	if (entry->is_dir) {
		if (entry->filename_len != php_stream_write(p->filefp, entry->filename, entry->filename_len)) {
			spprintf(p->error, 0, "unable to write filename to local directory entry for directory \"%s\" while creating zip-based phar \"%s\"", entry->filename, entry->phar->fname);
			return ZEND_HASH_APPLY_STOP;
		}

		if (1 != php_stream_write(p->filefp, "/", 1)) {
			spprintf(p->error, 0, "unable to write filename to local directory entry for directory \"%s\" while creating zip-based phar \"%s\"", entry->filename, entry->phar->fname);
			return ZEND_HASH_APPLY_STOP;
		}

		if (entry->filename_len != php_stream_write(p->centralfp, entry->filename, entry->filename_len)) {
			spprintf(p->error, 0, "unable to write filename to central directory entry for directory \"%s\" while creating zip-based phar \"%s\"", entry->filename, entry->phar->fname);
			return ZEND_HASH_APPLY_STOP;
		}

		if (1 != php_stream_write(p->centralfp, "/", 1)) {
			spprintf(p->error, 0, "unable to write filename to central directory entry for directory \"%s\" while creating zip-based phar \"%s\"", entry->filename, entry->phar->fname);
			return ZEND_HASH_APPLY_STOP;
		}
	} else {
		if (entry->filename_len != php_stream_write(p->filefp, entry->filename, entry->filename_len)) {
			spprintf(p->error, 0, "unable to write filename to local directory entry for file \"%s\" while creating zip-based phar \"%s\"", entry->filename, entry->phar->fname);
			return ZEND_HASH_APPLY_STOP;
		}

		if (entry->filename_len != php_stream_write(p->centralfp, entry->filename, entry->filename_len)) {
			spprintf(p->error, 0, "unable to write filename to central directory entry for file \"%s\" while creating zip-based phar \"%s\"", entry->filename, entry->phar->fname);
			return ZEND_HASH_APPLY_STOP;
		}
	}

	if (sizeof(perms) != php_stream_write(p->filefp, (char *)&perms, sizeof(perms))) {
		spprintf(p->error, 0, "unable to write local extra permissions file header of file \"%s\" to zip-based phar \"%s\"", entry->filename, entry->phar->fname);
		return ZEND_HASH_APPLY_STOP;
	}

	if (sizeof(perms) != php_stream_write(p->centralfp, (char *)&perms, sizeof(perms))) {
		spprintf(p->error, 0, "unable to write central extra permissions file header of file \"%s\" to zip-based phar \"%s\"", entry->filename, entry->phar->fname);
		return ZEND_HASH_APPLY_STOP;
	}

	if (!not_really_modified && entry->is_modified) {
		if (entry->cfp) {
			if (SUCCESS != php_stream_copy_to_stream_ex(entry->cfp, p->filefp, entry->compressed_filesize, NULL)) {
				spprintf(p->error, 0, "unable to write compressed contents of file \"%s\" in zip-based phar \"%s\"", entry->filename, entry->phar->fname);
				return ZEND_HASH_APPLY_STOP;
			}

			php_stream_close(entry->cfp);
			entry->cfp = NULL;
		} else {
			if (FAILURE == phar_open_entry_fp(entry, p->error, 0)) {
				return ZEND_HASH_APPLY_STOP;
			}

			phar_seek_efp(entry, 0, SEEK_SET, 0, 0);

			if (SUCCESS != php_stream_copy_to_stream_ex(phar_get_efp(entry, 0), p->filefp, entry->uncompressed_filesize, NULL)) {
				spprintf(p->error, 0, "unable to write contents of file \"%s\" in zip-based phar \"%s\"", entry->filename, entry->phar->fname);
				return ZEND_HASH_APPLY_STOP;
			}
		}

		if (entry->fp_type == PHAR_MOD && entry->fp != entry->phar->fp && entry->fp != entry->phar->ufp && entry->fp_refcount == 0) {
			php_stream_close(entry->fp);
		}

		entry->is_modified = 0;
	} else {
		entry->is_modified = 0;
		if (entry->fp_refcount) {
			 
			switch (entry->fp_type) {
				case PHAR_FP:
					p->free_fp = 0;
					break;
				case PHAR_UFP:
					p->free_ufp = 0;
				default:
					break;
			}
		}

		if (!entry->is_dir && entry->compressed_filesize && SUCCESS != php_stream_copy_to_stream_ex(p->old, p->filefp, entry->compressed_filesize, NULL)) {
			spprintf(p->error, 0, "unable to copy contents of file \"%s\" while creating zip-based phar \"%s\"", entry->filename, entry->phar->fname);
			return ZEND_HASH_APPLY_STOP;
		}
	}

	entry->fp = NULL;
	entry->offset = entry->offset_abs = offset;
	entry->fp_type = PHAR_FP;

	if (entry->metadata_str.s) {
		if (ZSTR_LEN(entry->metadata_str.s) != php_stream_write(p->centralfp, ZSTR_VAL(entry->metadata_str.s), ZSTR_LEN(entry->metadata_str.s))) {
			spprintf(p->error, 0, "unable to write metadata as file comment for file \"%s\" while creating zip-based phar \"%s\"", entry->filename, entry->phar->fname);
			smart_str_free(&entry->metadata_str);
			return ZEND_HASH_APPLY_STOP;
		}

		smart_str_free(&entry->metadata_str);
	}

	return ZEND_HASH_APPLY_KEEP;
}
 