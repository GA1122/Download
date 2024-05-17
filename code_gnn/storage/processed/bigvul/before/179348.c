  _archive_write_data(struct archive *_a, const void *buff, size_t s)
  {
  	struct archive_write *a = (struct archive_write *)_a;
// 	const size_t max_write = INT_MAX;
// 
  	archive_check_magic(&a->archive, ARCHIVE_WRITE_MAGIC,
  	    ARCHIVE_STATE_DATA, "archive_write_data");
// 	 
// 	if (s > max_write)
// 		s = max_write;
  	archive_clear_error(&a->archive);
  	return ((a->format_write_data)(a, buff, s));
  }