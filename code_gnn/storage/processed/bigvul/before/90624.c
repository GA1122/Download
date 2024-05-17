archive_read_format_zip_read_data(struct archive_read *a,
    const void **buff, size_t *size, int64_t *offset)
{
	int r;
	struct zip *zip = (struct zip *)(a->format->data);

	if (zip->has_encrypted_entries ==
			ARCHIVE_READ_FORMAT_ENCRYPTION_DONT_KNOW) {
		zip->has_encrypted_entries = 0;
	}

	*offset = zip->entry_uncompressed_bytes_read;
	*size = 0;
	*buff = NULL;

	 
	if (zip->end_of_entry)
		return (ARCHIVE_EOF);

	 
	if (AE_IFREG != (zip->entry->mode & AE_IFMT))
		return (ARCHIVE_EOF);

	__archive_read_consume(a, zip->unconsumed);
	zip->unconsumed = 0;

	if (zip->init_decryption) {
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

	switch(zip->entry->compression) {
	case 0:   
		r =  zip_read_data_none(a, buff, size, offset);
		break;
#ifdef HAVE_BZLIB_H
	case 12:  
		r = zip_read_data_zipx_bzip2(a, buff, size, offset);
		break;
#endif
#if HAVE_LZMA_H && HAVE_LIBLZMA
	case 14:  
		r = zip_read_data_zipx_lzma_alone(a, buff, size, offset);
		break;
	case 95:  
		r = zip_read_data_zipx_xz(a, buff, size, offset);
		break;
#endif
	 
	case 98:  
		r = zip_read_data_zipx_ppmd(a, buff, size, offset);
		break;

#ifdef HAVE_ZLIB_H
	case 8:  
		r =  zip_read_data_deflate(a, buff, size, offset);
		break;
#endif
	default:  
		 
		archive_set_error(&a->archive, ARCHIVE_ERRNO_FILE_FORMAT,
		    "Unsupported ZIP compression method (%d: %s)",
		    zip->entry->compression, compression_name(zip->entry->compression));
		 
		return (ARCHIVE_FAILED);
		break;
	}
	if (r != ARCHIVE_OK)
		return (r);
	 
	if (*size)
		zip->entry_crc32 = zip->crc32func(zip->entry_crc32, *buff,
		    (unsigned)*size);
	 
	if (zip->end_of_entry) {
		 
		if (zip->entry->compressed_size !=
		    zip->entry_compressed_bytes_read) {
			archive_set_error(&a->archive, ARCHIVE_ERRNO_MISC,
			    "ZIP compressed data is wrong size "
			    "(read %jd, expected %jd)",
			    (intmax_t)zip->entry_compressed_bytes_read,
			    (intmax_t)zip->entry->compressed_size);
			return (ARCHIVE_WARN);
		}
		 
		if ((zip->entry->uncompressed_size & UINT32_MAX)
		    != (zip->entry_uncompressed_bytes_read & UINT32_MAX)) {
			archive_set_error(&a->archive, ARCHIVE_ERRNO_MISC,
			    "ZIP uncompressed data is wrong size "
			    "(read %jd, expected %jd)\n",
			    (intmax_t)zip->entry_uncompressed_bytes_read,
			    (intmax_t)zip->entry->uncompressed_size);
			return (ARCHIVE_WARN);
		}
		 
		if ((!zip->hctx_valid ||
		      zip->entry->aes_extra.vendor != AES_VENDOR_AE_2) &&
		   zip->entry->crc32 != zip->entry_crc32
		    && !zip->ignore_crc32) {
			archive_set_error(&a->archive, ARCHIVE_ERRNO_MISC,
			    "ZIP bad CRC: 0x%lx should be 0x%lx",
			    (unsigned long)zip->entry_crc32,
			    (unsigned long)zip->entry->crc32);
			return (ARCHIVE_WARN);
		}
	}

	return (ARCHIVE_OK);
}
