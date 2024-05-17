archive_read_format_lha_read_data_skip(struct archive_read *a)
{
	struct lha *lha;
	int64_t bytes_skipped;

	lha = (struct lha *)(a->format->data);

	if (lha->entry_unconsumed) {
		 
		__archive_read_consume(a, lha->entry_unconsumed);
		lha->entry_unconsumed = 0;
	}

	 
	if (lha->end_of_entry_cleanup)
		return (ARCHIVE_OK);

	 
	bytes_skipped = __archive_read_consume(a, lha->entry_bytes_remaining);
	if (bytes_skipped < 0)
		return (ARCHIVE_FATAL);

	 
	lha->end_of_entry_cleanup = lha->end_of_entry = 1;
	return (ARCHIVE_OK);
}
