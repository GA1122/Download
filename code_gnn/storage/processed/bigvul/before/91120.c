static void ip_handle_martian_source(struct net_device *dev,
				     struct in_device *in_dev,
				     struct sk_buff *skb,
				     __be32 daddr,
				     __be32 saddr)
{
	RT_CACHE_STAT_INC(in_martian_src);
#ifdef CONFIG_IP_ROUTE_VERBOSE
	if (IN_DEV_LOG_MARTIANS(in_dev) && net_ratelimit()) {
		 
		pr_warn("martian source %pI4 from %pI4, on dev %s\n",
			&daddr, &saddr, dev->name);
		if (dev->hard_header_len && skb_mac_header_was_set(skb)) {
			print_hex_dump(KERN_WARNING, "ll header: ",
				       DUMP_PREFIX_OFFSET, 16, 1,
				       skb_mac_header(skb),
				       dev->hard_header_len, false);
		}
	}
#endif
}