static int run_filter(struct tap_filter *filter, const struct sk_buff *skb)
{
	 
	struct ethhdr *eh = (struct ethhdr *) skb->data;
	int i;

	 
	for (i = 0; i < filter->count; i++)
		if (!compare_ether_addr(eh->h_dest, filter->addr[i]))
			return 1;

	 
	if (is_multicast_ether_addr(eh->h_dest))
		return addr_hash_test(filter->mask, eh->h_dest);

	return 0;
}
