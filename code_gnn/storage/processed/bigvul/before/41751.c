static void addrconf_addr_gen(struct inet6_dev *idev, bool prefix_route)
{
	if (idev->addr_gen_mode == IN6_ADDR_GEN_MODE_EUI64) {
		struct in6_addr addr;

		ipv6_addr_set(&addr,  htonl(0xFE800000), 0, 0, 0);
		 
		if (ipv6_generate_eui64(addr.s6_addr + 8, idev->dev) == 0)
			addrconf_add_linklocal(idev, &addr);
		else if (prefix_route)
			addrconf_prefix_route(&addr, 64, idev->dev, 0, 0);
	}
}
