client_skip_proxy(struct archive_read_filter *self, int64_t request)
{
	if (request < 0)
		__archive_errx(1, "Negative skip requested.");
	if (request == 0)
		return 0;

	if (self->archive->client.skipper != NULL) {
		 
		int64_t skip_limit = (int64_t)1 << 30;
		int64_t total = 0;
		for (;;) {
			int64_t get, ask = request;
			if (ask > skip_limit)
				ask = skip_limit;
			get = (self->archive->client.skipper)
				(&self->archive->archive, self->data, ask);
			if (get == 0)
				return (total);
			request -= get;
			total += get;
		}
	} else if (self->archive->client.seeker != NULL
		&& request > 64 * 1024) {
		 
		int64_t before = self->position;
		int64_t after = (self->archive->client.seeker)
		    (&self->archive->archive, self->data, request, SEEK_CUR);
		if (after != before + request)
			return ARCHIVE_FATAL;
		return after - before;
	}
	return 0;
}
