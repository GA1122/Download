static __net_init int rt_genid_init(struct net *net)
{
	atomic_set(&net->ipv4.rt_genid, 0);
	atomic_set(&net->fnhe_genid, 0);
	get_random_bytes(&net->ipv4.dev_addr_genid,
			 sizeof(net->ipv4.dev_addr_genid));
	return 0;
}