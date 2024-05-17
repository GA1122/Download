struct in6_addr *ndp_msg_opt_rdnss_addr(struct ndp_msg *msg, int offset,
					int addr_index)
{
	static struct in6_addr addr;
	struct __nd_opt_rdnss *rdnss =
			ndp_msg_payload_opts_offset(msg, offset);
	size_t len = rdnss->nd_opt_rdnss_len << 3;  

	len -= in_struct_offset(struct __nd_opt_rdnss, nd_opt_rdnss_addresses);
	if ((addr_index + 1) * sizeof(addr) > len)
		return NULL;
	memcpy(&addr, &rdnss->nd_opt_rdnss_addresses[addr_index * sizeof(addr)],
	       sizeof(addr));
	return &addr;
}