static int unix_mkname(struct sockaddr_un *sunaddr, int len, unsigned int *hashp)
{
	if (len <= sizeof(short) || len > sizeof(*sunaddr))
		return -EINVAL;
	if (!sunaddr || sunaddr->sun_family != AF_UNIX)
		return -EINVAL;
	if (sunaddr->sun_path[0]) {
		 
		((char *)sunaddr)[len] = 0;
		len = strlen(sunaddr->sun_path)+1+sizeof(short);
		return len;
	}

	*hashp = unix_hash_fold(csum_partial(sunaddr, len, 0));
	return len;
}
