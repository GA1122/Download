archive_write_client_close(struct archive_write_filter *f)
{
	struct archive_write *a = (struct archive_write *)f->archive;
	struct archive_none *state = (struct archive_none *)f->data;
	ssize_t block_length;
	ssize_t target_block_length;
	ssize_t bytes_written;
	int ret = ARCHIVE_OK;

	 
	if (state->next != state->buffer) {
		block_length = state->buffer_size - state->avail;

		 
		if (a->bytes_in_last_block <= 0)
			 
			target_block_length = a->bytes_per_block;
		else
			 
			target_block_length = a->bytes_in_last_block *
			    ( (block_length + a->bytes_in_last_block - 1) /
			        a->bytes_in_last_block);
		if (target_block_length > a->bytes_per_block)
			target_block_length = a->bytes_per_block;
		if (block_length < target_block_length) {
			memset(state->next, 0,
			    target_block_length - block_length);
			block_length = target_block_length;
		}
		bytes_written = (a->client_writer)(&a->archive,
		    a->client_data, state->buffer, block_length);
		ret = bytes_written <= 0 ? ARCHIVE_FATAL : ARCHIVE_OK;
	}
	if (a->client_closer)
		(*a->client_closer)(&a->archive, a->client_data);
	free(state->buffer);
	free(state);
	 
	f->close = NULL;
	a->client_data = NULL;
	return (ret);
}