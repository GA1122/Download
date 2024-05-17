extract_hostname(const char *unc)
{
	const char *src;
	char *dst, *delim;
	unsigned int len;

	 
	 
	src = unc + 2;

	 
	delim = strchr(src, '\\');
	if (!delim)
		return ERR_PTR(-EINVAL);

	len = delim - src;
	dst = kmalloc((len + 1), GFP_KERNEL);
	if (dst == NULL)
		return ERR_PTR(-ENOMEM);

	memcpy(dst, src, len);
	dst[len] = '\0';

	return dst;
}
