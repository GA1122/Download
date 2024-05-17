zip_read_data_zipx_ppmd(struct archive_read *a, const void **buff,
    size_t *size, int64_t *offset)
{
	struct zip* zip = (struct zip *)(a->format->data);
	int ret;
	size_t consumed_bytes = 0;
	ssize_t bytes_avail = 0;

	(void) offset;  

	 
	if(!zip->decompress_init) {
		ret = zipx_ppmd8_init(a, zip);
		if(ret != ARCHIVE_OK)
			return ret;
	}

	 
	(void) __archive_read_ahead(a, 1, &bytes_avail);
	if(bytes_avail < 0) {
		archive_set_error(&a->archive, ARCHIVE_ERRNO_FILE_FORMAT,
		    "Truncated PPMd8 file body");
		return (ARCHIVE_FATAL);
	}

	 
	zip->zipx_ppmd_read_compressed = 0;

	 
	do {
		int sym = __archive_ppmd8_functions.Ppmd8_DecodeSymbol(&zip->ppmd8);
		if(sym < 0) {
			zip->end_of_entry = 1;
			break;
		}

		 
		if(zip->ppmd8_stream_failed) {
			archive_set_error(&a->archive, ARCHIVE_ERRNO_FILE_FORMAT,
				"Truncated PPMd8 file body");
			return (ARCHIVE_FATAL);
		}

		zip->uncompressed_buffer[consumed_bytes] = (uint8_t) sym;
		++consumed_bytes;
	} while(consumed_bytes < zip->uncompressed_buffer_size);

	 
	*buff = zip->uncompressed_buffer;
	*size = consumed_bytes;

	 
	zip->entry_bytes_remaining -= zip->zipx_ppmd_read_compressed;
	zip->entry_compressed_bytes_read += zip->zipx_ppmd_read_compressed;
	zip->entry_uncompressed_bytes_read += consumed_bytes;

	 
	if(zip->end_of_entry) {
		__archive_ppmd8_functions.Ppmd8_Free(&zip->ppmd8);
		zip->ppmd8_valid = 0;
	}

	 
	ret = consume_optional_marker(a, zip);
	if (ret != ARCHIVE_OK)
		return ret;

	return ARCHIVE_OK;
}
