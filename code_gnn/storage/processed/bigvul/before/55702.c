archive_read_format_zip_read_data_skip_streamable(struct archive_read *a)
{
	struct zip *zip;
	int64_t bytes_skipped;

	zip = (struct zip *)(a->format->data);
	bytes_skipped = __archive_read_consume(a, zip->unconsumed);
	zip->unconsumed = 0;
	if (bytes_skipped < 0)
		return (ARCHIVE_FATAL);

	 
	if (zip->end_of_entry)
		return (ARCHIVE_OK);

	 
	if (0 == (zip->entry->zip_flags & ZIP_LENGTH_AT_END)
	    || zip->entry->compressed_size > 0) {
		 
		bytes_skipped = __archive_read_consume(a,
					zip->entry_bytes_remaining);
		if (bytes_skipped < 0)
			return (ARCHIVE_FATAL);
		return (ARCHIVE_OK);
	}

	if (zip->init_decryption) {
		int r;

		zip->has_encrypted_entries = 1;
		if (zip->entry->zip_flags & ZIP_STRONG_ENCRYPTED)
			r = read_decryption_header(a);
		else if (zip->entry->compression == WINZIP_AES_ENCRYPTION)
			r = init_WinZip_AES_decryption(a);
		else
			r = init_traditional_PKWARE_decryption(a);
		if (r != ARCHIVE_OK)
			return (r);
		zip->init_decryption = 0;
	}

	 
	 
	switch (zip->entry->compression) {
#ifdef HAVE_ZLIB_H
	case 8:  
		while (!zip->end_of_entry) {
			int64_t offset = 0;
			const void *buff = NULL;
			size_t size = 0;
			int r;
			r =  zip_read_data_deflate(a, &buff, &size, &offset);
			if (r != ARCHIVE_OK)
				return (r);
		}
		return ARCHIVE_OK;
#endif
	default:  
		 
		for (;;) {
			const char *p, *buff;
			ssize_t bytes_avail;
			buff = __archive_read_ahead(a, 16, &bytes_avail);
			if (bytes_avail < 16) {
				archive_set_error(&a->archive,
				    ARCHIVE_ERRNO_FILE_FORMAT,
				    "Truncated ZIP file data");
				return (ARCHIVE_FATAL);
			}
			p = buff;
			while (p <= buff + bytes_avail - 16) {
				if (p[3] == 'P') { p += 3; }
				else if (p[3] == 'K') { p += 2; }
				else if (p[3] == '\007') { p += 1; }
				else if (p[3] == '\010' && p[2] == '\007'
				    && p[1] == 'K' && p[0] == 'P') {
					if (zip->entry->flags & LA_USED_ZIP64)
						__archive_read_consume(a,
						    p - buff + 24);
					else
						__archive_read_consume(a,
						    p - buff + 16);
					return ARCHIVE_OK;
				} else { p += 4; }
			}
			__archive_read_consume(a, p - buff);
		}
	}
}
