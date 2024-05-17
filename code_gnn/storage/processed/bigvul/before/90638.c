zip_read_data_zipx_lzma_alone(struct archive_read *a, const void **buff,
    size_t *size, int64_t *offset)
{
	struct zip* zip = (struct zip *)(a->format->data);
	int ret;
	lzma_ret lz_ret;
	const void* compressed_buf;
	ssize_t bytes_avail, in_bytes, to_consume;

	(void) offset;  

	 
	if (!zip->decompress_init) {
		ret = zipx_lzma_alone_init(a, zip);
		if (ret != ARCHIVE_OK)
			return (ret);
	}

	 
	compressed_buf = __archive_read_ahead(a, 1, &bytes_avail);
	if (bytes_avail < 0) {
		archive_set_error(&a->archive, ARCHIVE_ERRNO_FILE_FORMAT,
		    "Truncated lzma file body");
		return (ARCHIVE_FATAL);
	}

	 
	in_bytes = zipmin(zip->entry_bytes_remaining, bytes_avail);

	zip->zipx_lzma_stream.next_in = compressed_buf;
	zip->zipx_lzma_stream.avail_in = in_bytes;
	zip->zipx_lzma_stream.total_in = 0;
	zip->zipx_lzma_stream.next_out = zip->uncompressed_buffer;
	zip->zipx_lzma_stream.avail_out =
		 
		zipmin((int64_t) zip->uncompressed_buffer_size,
		    zip->entry->uncompressed_size -
		    zip->entry_uncompressed_bytes_read);
	zip->zipx_lzma_stream.total_out = 0;

	 
	lz_ret = lzma_code(&zip->zipx_lzma_stream, LZMA_RUN);
	switch(lz_ret) {
		case LZMA_DATA_ERROR:
			archive_set_error(&a->archive, ARCHIVE_ERRNO_MISC,
			    "lzma data error (error %d)", (int) lz_ret);
			return (ARCHIVE_FATAL);

		case LZMA_OK:
			break;

		default:
			archive_set_error(&a->archive, ARCHIVE_ERRNO_MISC,
			    "lzma unknown error %d", (int) lz_ret);
			return (ARCHIVE_FATAL);
	}

	to_consume = zip->zipx_lzma_stream.total_in;

	 
	__archive_read_consume(a, to_consume);
	zip->entry_bytes_remaining -= to_consume;
	zip->entry_compressed_bytes_read += to_consume;
	zip->entry_uncompressed_bytes_read += zip->zipx_lzma_stream.total_out;

	if(zip->entry_bytes_remaining == 0) {
		zip->end_of_entry = 1;
	}

	 
	*size = zip->zipx_lzma_stream.total_out;
	*buff = zip->uncompressed_buffer;

	 
	ret = consume_optional_marker(a, zip);
	if (ret != ARCHIVE_OK)
		return (ret);

	 
	if(zip->end_of_entry) {
		lzma_end(&zip->zipx_lzma_stream);
		zip->zipx_lzma_valid = 0;
	}

	 
	return (ARCHIVE_OK);
}
