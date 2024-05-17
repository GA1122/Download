_archive_read_next_header2(struct archive *_a, struct archive_entry *entry)
{
	struct archive_read *a = (struct archive_read *)_a;
	int r1 = ARCHIVE_OK, r2;

	archive_check_magic(_a, ARCHIVE_READ_MAGIC,
	    ARCHIVE_STATE_HEADER | ARCHIVE_STATE_DATA,
	    "archive_read_next_header");

	archive_entry_clear(entry);
	archive_clear_error(&a->archive);

	 
	if (a->archive.state == ARCHIVE_STATE_DATA) {
		r1 = archive_read_data_skip(&a->archive);
		if (r1 == ARCHIVE_EOF)
			archive_set_error(&a->archive, EIO,
			    "Premature end-of-file.");
		if (r1 == ARCHIVE_EOF || r1 == ARCHIVE_FATAL) {
			a->archive.state = ARCHIVE_STATE_FATAL;
			return (ARCHIVE_FATAL);
		}
	}

	 
	a->header_position = a->filter->position;

	++_a->file_count;
	r2 = (a->format->read_header)(a, entry);

	 
	switch (r2) {
	case ARCHIVE_EOF:
		a->archive.state = ARCHIVE_STATE_EOF;
		--_a->file_count; 
		break;
	case ARCHIVE_OK:
		a->archive.state = ARCHIVE_STATE_DATA;
		break;
	case ARCHIVE_WARN:
		a->archive.state = ARCHIVE_STATE_DATA;
		break;
	case ARCHIVE_RETRY:
		break;
	case ARCHIVE_FATAL:
		a->archive.state = ARCHIVE_STATE_FATAL;
		break;
	}

	a->read_data_output_offset = 0;
	a->read_data_remaining = 0;
	a->read_data_is_posix_read = 0;
	a->read_data_requested = 0;
	a->data_start_node = a->client.cursor;
	 
	return (r2 < r1 || r2 == ARCHIVE_EOF) ? r2 : r1;
}
