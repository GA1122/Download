archive_read_format_7zip_read_data_skip(struct archive_read *a)
{
	struct _7zip *zip;
	int64_t bytes_skipped;

	zip = (struct _7zip *)(a->format->data);

	if (zip->pack_stream_bytes_unconsumed)
		read_consume(a);

	 
	if (zip->end_of_entry)
		return (ARCHIVE_OK);

	 
	bytes_skipped = skip_stream(a, (size_t)zip->entry_bytes_remaining);
	if (bytes_skipped < 0)
		return (ARCHIVE_FATAL);
	zip->entry_bytes_remaining = 0;

	 
	zip->end_of_entry = 1;
	return (ARCHIVE_OK);
}
