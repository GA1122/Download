void *realloc_strcat(char *ptr, char *s)
{
	size_t old = 0, len = strlen(s);
	char *ret;

	if (!len)
		return ptr;
	if (ptr)
		old = strlen(ptr);

	len += old + 1;
	align_len(&len);

	ret = malloc(len);
	if (unlikely(!ret))
		quithere(1, "Failed to malloc");

	if (ptr) {
		sprintf(ret, "%s%s", ptr, s);
		free(ptr);
	} else
		sprintf(ret, "%s", s);
	return ret;
}
