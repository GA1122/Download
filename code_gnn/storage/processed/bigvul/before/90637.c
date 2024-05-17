zip_read_data_zipx_bzip2(struct archive_read *a, const void **buff,
    size_t *size, int64_t *offset)
{
	struct zip *zip = (struct zip *)(a->format->data);
	ssize_t bytes_avail = 0, in_bytes, to_consume;
	const void *compressed_buff;
	int r;
	uint64_t total_out;

	(void) offset;  

	 
	if(!zip->decompress_init) {
		r = zipx_bzip2_init(a, zip);
		if(r != ARCHIVE_OK)
			return r;
	}

	 
	compressed_buff = __archive_read_ahead(a, 1, &bytes_avail);
	if(bytes_avail < 0) {
		archive_set_error(&a->archive, ARCHIVE_ERRNO_FILE_FORMAT,
		    "Truncated bzip2 file body");
		return (ARCHIVE_FATAL);
	}

	in_bytes = zipmin(zip->entry_bytes_remaining, bytes_avail);
	if(in_bytes < 1) {
		 

		archive_set_error(&a->archive, ARCHIVE_ERRNO_FILE_FORMAT,
		    "Truncated bzip2 file body");
		return (ARCHIVE_FATAL);
	}

	 
	zip->bzstream.next_in = (char*)(uintptr_t) compressed_buff;
	zip->bzstream.avail_in = in_bytes;
	zip->bzstream.total_in_hi32 = 0;
	zip->bzstream.total_in_lo32 = 0;
	zip->bzstream.next_out = (char*) zip->uncompressed_buffer;
	zip->bzstream.avail_out = zip->uncompressed_buffer_size;
	zip->bzstream.total_out_hi32 = 0;
	zip->bzstream.total_out_lo32 = 0;

	 
	r = BZ2_bzDecompress(&zip->bzstream);
	switch(r) {
		case BZ_STREAM_END:
			 
			switch(BZ2_bzDecompressEnd(&zip->bzstream)) {
				case BZ_OK:
					break;
				default:
					archive_set_error(&a->archive, ARCHIVE_ERRNO_MISC,
					    "Failed to clean up bzip2 decompressor");
					return ARCHIVE_FATAL;
			}

			zip->end_of_entry = 1;
			break;
		case BZ_OK:
			 
			break;
		default:
			archive_set_error(&a->archive, ARCHIVE_ERRNO_MISC,
			    "bzip2 decompression failed");
			return ARCHIVE_FATAL;
	}

	 
	to_consume = zip->bzstream.total_in_lo32;
	__archive_read_consume(a, to_consume);

	total_out = ((uint64_t) zip->bzstream.total_out_hi32 << 32) +
	    zip->bzstream.total_out_lo32;

	zip->entry_bytes_remaining -= to_consume;
	zip->entry_compressed_bytes_read += to_consume;
	zip->entry_uncompressed_bytes_read += total_out;

	 
	*size = total_out;
	*buff = zip->uncompressed_buffer;

	 
	r = consume_optional_marker(a, zip);
	if(r != ARCHIVE_OK)
		return r;

	return ARCHIVE_OK;
}