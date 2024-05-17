int FAST_FUNC udhcp_str2nip(const char *str, void *arg)
{
	len_and_sockaddr *lsa;

	lsa = host_and_af2sockaddr(str, 0, AF_INET);
	if (!lsa)
		return 0;
	 
	move_to_unaligned32((uint32_t*)arg, lsa->u.sin.sin_addr.s_addr);
	free(lsa);
	return 1;
}
