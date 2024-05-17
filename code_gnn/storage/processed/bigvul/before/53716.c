static u32 cookie_hash(const struct in6_addr *saddr, const struct in6_addr *daddr,
		       __be16 sport, __be16 dport, u32 count, int c)
{
	__u32 *tmp;

	net_get_random_once(syncookie6_secret, sizeof(syncookie6_secret));

	tmp  = this_cpu_ptr(ipv6_cookie_scratch);

	 
	memcpy(tmp + 10, syncookie6_secret[c], 44);
	memcpy(tmp, saddr, 16);
	memcpy(tmp + 4, daddr, 16);
	tmp[8] = ((__force u32)sport << 16) + (__force u32)dport;
	tmp[9] = count;
	sha_transform(tmp + 16, (__u8 *)tmp, tmp + 16 + 5);

	return tmp[17];
}