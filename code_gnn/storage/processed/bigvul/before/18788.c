static int __init inet_init(void)
{
	struct sk_buff *dummy_skb;
	struct inet_protosw *q;
	struct list_head *r;
	int rc = -EINVAL;

	BUILD_BUG_ON(sizeof(struct inet_skb_parm) > sizeof(dummy_skb->cb));

	sysctl_local_reserved_ports = kzalloc(65536 / 8, GFP_KERNEL);
	if (!sysctl_local_reserved_ports)
		goto out;

	rc = proto_register(&tcp_prot, 1);
	if (rc)
		goto out_free_reserved_ports;

	rc = proto_register(&udp_prot, 1);
	if (rc)
		goto out_unregister_tcp_proto;

	rc = proto_register(&raw_prot, 1);
	if (rc)
		goto out_unregister_udp_proto;

	 

	(void)sock_register(&inet_family_ops);

#ifdef CONFIG_SYSCTL
	ip_static_sysctl_init();
#endif

	 

	if (inet_add_protocol(&icmp_protocol, IPPROTO_ICMP) < 0)
		printk(KERN_CRIT "inet_init: Cannot add ICMP protocol\n");
	if (inet_add_protocol(&udp_protocol, IPPROTO_UDP) < 0)
		printk(KERN_CRIT "inet_init: Cannot add UDP protocol\n");
	if (inet_add_protocol(&tcp_protocol, IPPROTO_TCP) < 0)
		printk(KERN_CRIT "inet_init: Cannot add TCP protocol\n");
#ifdef CONFIG_IP_MULTICAST
	if (inet_add_protocol(&igmp_protocol, IPPROTO_IGMP) < 0)
		printk(KERN_CRIT "inet_init: Cannot add IGMP protocol\n");
#endif

	 
	for (r = &inetsw[0]; r < &inetsw[SOCK_MAX]; ++r)
		INIT_LIST_HEAD(r);

	for (q = inetsw_array; q < &inetsw_array[INETSW_ARRAY_LEN]; ++q)
		inet_register_protosw(q);

	 

	arp_init();

	 

	ip_init();

	tcp_v4_init();

	 
	tcp_init();

	 
	udp_init();

	 
	udplite4_register();

	 

	if (icmp_init() < 0)
		panic("Failed to create the ICMP control socket.\n");

	 
#if defined(CONFIG_IP_MROUTE)
	if (ip_mr_init())
		printk(KERN_CRIT "inet_init: Cannot init ipv4 mroute\n");
#endif
	 

	if (init_ipv4_mibs())
		printk(KERN_CRIT "inet_init: Cannot init ipv4 mibs\n");

	ipv4_proc_init();

	ipfrag_init();

	dev_add_pack(&ip_packet_type);

	rc = 0;
out:
	return rc;
out_unregister_udp_proto:
	proto_unregister(&udp_prot);
out_unregister_tcp_proto:
	proto_unregister(&tcp_prot);
out_free_reserved_ports:
	kfree(sysctl_local_reserved_ports);
	goto out;
}
