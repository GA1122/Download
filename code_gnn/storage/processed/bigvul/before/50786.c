cleanup_backslash_1(char *p)
{
	int mb, dos;

	mb = dos = 0;
	while (*p) {
		if (*(unsigned char *)p > 127)
			mb = 1;
		if (*p == '\\') {
			 
			if (!mb)
				*p = '/';
			dos = 1;
		}
		p++;
	}
	if (!mb || !dos)
		return (0);
	return (-1);
}
