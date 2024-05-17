void fib_add_ifaddr(struct in_ifaddr *ifa)
{
	struct in_device *in_dev = ifa->ifa_dev;
	struct net_device *dev = in_dev->dev;
	struct in_ifaddr *prim = ifa;
	__be32 mask = ifa->ifa_mask;
	__be32 addr = ifa->ifa_local;
	__be32 prefix = ifa->ifa_address & mask;

	if (ifa->ifa_flags & IFA_F_SECONDARY) {
		prim = inet_ifa_byprefix(in_dev, prefix, mask);
		if (!prim) {
			pr_warn("%s: bug: prim == NULL\n", __func__);
			return;
		}
	}

	fib_magic(RTM_NEWROUTE, RTN_LOCAL, addr, 32, prim);

	if (!(dev->flags & IFF_UP))
		return;

	 
	if (ifa->ifa_broadcast && ifa->ifa_broadcast != htonl(0xFFFFFFFF))
		fib_magic(RTM_NEWROUTE, RTN_BROADCAST, ifa->ifa_broadcast, 32, prim);

	if (!ipv4_is_zeronet(prefix) && !(ifa->ifa_flags & IFA_F_SECONDARY) &&
	    (prefix != addr || ifa->ifa_prefixlen < 32)) {
		if (!(ifa->ifa_flags & IFA_F_NOPREFIXROUTE))
			fib_magic(RTM_NEWROUTE,
				  dev->flags & IFF_LOOPBACK ? RTN_LOCAL : RTN_UNICAST,
				  prefix, ifa->ifa_prefixlen, prim);

		 
		if (ifa->ifa_prefixlen < 31) {
			fib_magic(RTM_NEWROUTE, RTN_BROADCAST, prefix, 32, prim);
			fib_magic(RTM_NEWROUTE, RTN_BROADCAST, prefix | ~mask,
				  32, prim);
		}
	}
}