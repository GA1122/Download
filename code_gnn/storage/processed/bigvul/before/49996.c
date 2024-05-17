__archive_read_filter_ahead(struct archive_read_filter *filter,
    size_t min, ssize_t *avail)
{
	ssize_t bytes_read;
	size_t tocopy;

	if (filter->fatal) {
		if (avail)
			*avail = ARCHIVE_FATAL;
		return (NULL);
	}

	 
	for (;;) {

		 
		if (filter->avail >= min && filter->avail > 0) {
			if (avail != NULL)
				*avail = filter->avail;
			return (filter->next);
		}

		 
		if (filter->client_total >= filter->client_avail + filter->avail
		    && filter->client_avail + filter->avail >= min) {
			 
			filter->client_avail += filter->avail;
			filter->client_next -= filter->avail;
			 
			filter->avail = 0;
			filter->next = filter->buffer;
			 
			if (avail != NULL)
				*avail = filter->client_avail;
			return (filter->client_next);
		}

		 
		if (filter->next > filter->buffer &&
		    filter->next + min > filter->buffer + filter->buffer_size) {
			if (filter->avail > 0)
				memmove(filter->buffer, filter->next,
				    filter->avail);
			filter->next = filter->buffer;
		}

		 
		if (filter->client_avail <= 0) {
			if (filter->end_of_file) {
				if (avail != NULL)
					*avail = 0;
				return (NULL);
			}
			bytes_read = (filter->read)(filter,
			    &filter->client_buff);
			if (bytes_read < 0) {		 
				filter->client_total = filter->client_avail = 0;
				filter->client_next =
				    filter->client_buff = NULL;
				filter->fatal = 1;
				if (avail != NULL)
					*avail = ARCHIVE_FATAL;
				return (NULL);
			}
			if (bytes_read == 0) {
				 
				if (filter->archive->client.cursor !=
				      filter->archive->client.nodes - 1) {
					if (client_switch_proxy(filter,
					    filter->archive->client.cursor + 1)
					    == ARCHIVE_OK)
						continue;
				}
				 
				filter->client_total = filter->client_avail = 0;
				filter->client_next =
				    filter->client_buff = NULL;
				filter->end_of_file = 1;
				 
				if (avail != NULL)
					*avail = filter->avail;
				return (NULL);
			}
			filter->client_total = bytes_read;
			filter->client_avail = filter->client_total;
			filter->client_next = filter->client_buff;
		} else {
			 

			 
			if (min > filter->buffer_size) {
				size_t s, t;
				char *p;

				 
				s = t = filter->buffer_size;
				if (s == 0)
					s = min;
				while (s < min) {
					t *= 2;
					if (t <= s) {  
						archive_set_error(
						    &filter->archive->archive,
						    ENOMEM,
						    "Unable to allocate copy"
						    " buffer");
						filter->fatal = 1;
						if (avail != NULL)
							*avail = ARCHIVE_FATAL;
						return (NULL);
					}
					s = t;
				}
				 
				p = (char *)malloc(s);
				if (p == NULL) {
					archive_set_error(
						&filter->archive->archive,
						ENOMEM,
					    "Unable to allocate copy buffer");
					filter->fatal = 1;
					if (avail != NULL)
						*avail = ARCHIVE_FATAL;
					return (NULL);
				}
				 
				if (filter->avail > 0)
					memmove(p, filter->next, filter->avail);
				free(filter->buffer);
				filter->next = filter->buffer = p;
				filter->buffer_size = s;
			}

			 
			 
			tocopy = (filter->buffer + filter->buffer_size)
			    - (filter->next + filter->avail);
			 
			if (tocopy + filter->avail > min)
				tocopy = min - filter->avail;
			 
			if (tocopy > filter->client_avail)
				tocopy = filter->client_avail;

			memcpy(filter->next + filter->avail,
			    filter->client_next, tocopy);
			 
			filter->client_next += tocopy;
			filter->client_avail -= tocopy;
			 
			filter->avail += tocopy;
		}
	}
}
