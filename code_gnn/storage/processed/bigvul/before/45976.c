magic_descriptor(struct magic_set *ms, int fd)
{
	if (ms == NULL)
		return NULL;
	return file_or_fd(ms, NULL, fd);
}