static inline int audit_proctitle_rtrim(char *proctitle, int len)
{
	char *end = proctitle + len - 1;
	while (end > proctitle && !isprint(*end))
		end--;

	 
	len = end - proctitle + 1;
	len -= isprint(proctitle[len-1]) == 0;
	return len;
}
