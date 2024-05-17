static __net_init int inet_init_net(struct net *net)
{
	 
	seqlock_init(&net->ipv4.ip_local_ports.lock);
	net->ipv4.ip_local_ports.range[0] =  32768;
	net->ipv4.ip_local_ports.range[1] =  60999;

	seqlock_init(&net->ipv4.ping_group_range.lock);
	 
	net->ipv4.ping_group_range.range[0] = make_kgid(&init_user_ns, 1);
	net->ipv4.ping_group_range.range[1] = make_kgid(&init_user_ns, 0);
	return 0;
}
