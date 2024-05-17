magic_buffer(struct magic_set *ms, const void *buf, size_t nb)
{
	if (ms == NULL)
		return NULL;
	if (file_reset(ms) == -1)
		return NULL;
	 
	if (file_buffer(ms, -1, NULL, buf, nb) == -1) {
		return NULL;
	}
	return file_getbuffer(ms);
}