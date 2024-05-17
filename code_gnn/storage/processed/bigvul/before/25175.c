static __net_init int rt_genid_init(struct net *net)
{
	get_random_bytes(&net->ipv4.rt_genid,
			 sizeof(net->ipv4.rt_genid));
	get_random_bytes(&net->ipv4.dev_addr_genid,
			 sizeof(net->ipv4.dev_addr_genid));
	return 0;
}
