client_seek_proxy(struct archive_read_filter *self, int64_t offset, int whence)
{
	 
	if (self->archive->client.seeker == NULL) {
		archive_set_error(&self->archive->archive, ARCHIVE_ERRNO_MISC,
		    "Current client reader does not support seeking a device");
		return (ARCHIVE_FAILED);
	}
	return (self->archive->client.seeker)(&self->archive->archive,
	    self->data, offset, whence);
}
