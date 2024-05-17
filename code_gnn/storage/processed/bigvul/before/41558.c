static int __init inet6_init(void)
{
	struct list_head *r;
	int err = 0;

	sock_skb_cb_check_size(sizeof(struct inet6_skb_parm));

	 
	for (r = &inetsw6[0]; r < &inetsw6[SOCK_MAX]; ++r)
		INIT_LIST_HEAD(r);

	if (disable_ipv6_mod) {
		pr_info("Loaded, but administratively disabled, reboot required to enable\n");
		goto out;
	}

	err = proto_register(&tcpv6_prot, 1);
	if (err)
		goto out;

	err = proto_register(&udpv6_prot, 1);
	if (err)
		goto out_unregister_tcp_proto;

	err = proto_register(&udplitev6_prot, 1);
	if (err)
		goto out_unregister_udp_proto;

	err = proto_register(&rawv6_prot, 1);
	if (err)
		goto out_unregister_udplite_proto;

	err = proto_register(&pingv6_prot, 1);
	if (err)
		goto out_unregister_ping_proto;

	 
	err = rawv6_init();
	if (err)
		goto out_unregister_raw_proto;

	 
	err = sock_register(&inet6_family_ops);
	if (err)
		goto out_sock_register_fail;

	 

	err = register_pernet_subsys(&inet6_net_ops);
	if (err)
		goto register_pernet_fail;
	err = icmpv6_init();
	if (err)
		goto icmp_fail;
	err = ip6_mr_init();
	if (err)
		goto ipmr_fail;
	err = ndisc_init();
	if (err)
		goto ndisc_fail;
	err = igmp6_init();
	if (err)
		goto igmp_fail;

	ipv6_stub = &ipv6_stub_impl;

	err = ipv6_netfilter_init();
	if (err)
		goto netfilter_fail;
	 
#ifdef CONFIG_PROC_FS
	err = -ENOMEM;
	if (raw6_proc_init())
		goto proc_raw6_fail;
	if (udplite6_proc_init())
		goto proc_udplite6_fail;
	if (ipv6_misc_proc_init())
		goto proc_misc6_fail;
	if (if6_proc_init())
		goto proc_if6_fail;
#endif
	err = ip6_route_init();
	if (err)
		goto ip6_route_fail;
	err = ndisc_late_init();
	if (err)
		goto ndisc_late_fail;
	err = ip6_flowlabel_init();
	if (err)
		goto ip6_flowlabel_fail;
	err = addrconf_init();
	if (err)
		goto addrconf_fail;

	 
	err = ipv6_exthdrs_init();
	if (err)
		goto ipv6_exthdrs_fail;

	err = ipv6_frag_init();
	if (err)
		goto ipv6_frag_fail;

	 
	err = udpv6_init();
	if (err)
		goto udpv6_fail;

	err = udplitev6_init();
	if (err)
		goto udplitev6_fail;

	err = tcpv6_init();
	if (err)
		goto tcpv6_fail;

	err = ipv6_packet_init();
	if (err)
		goto ipv6_packet_fail;

	err = pingv6_init();
	if (err)
		goto pingv6_fail;

#ifdef CONFIG_SYSCTL
	err = ipv6_sysctl_register();
	if (err)
		goto sysctl_fail;
#endif
out:
	return err;

#ifdef CONFIG_SYSCTL
sysctl_fail:
	pingv6_exit();
#endif
pingv6_fail:
	ipv6_packet_cleanup();
ipv6_packet_fail:
	tcpv6_exit();
tcpv6_fail:
	udplitev6_exit();
udplitev6_fail:
	udpv6_exit();
udpv6_fail:
	ipv6_frag_exit();
ipv6_frag_fail:
	ipv6_exthdrs_exit();
ipv6_exthdrs_fail:
	addrconf_cleanup();
addrconf_fail:
	ip6_flowlabel_cleanup();
ip6_flowlabel_fail:
	ndisc_late_cleanup();
ndisc_late_fail:
	ip6_route_cleanup();
ip6_route_fail:
#ifdef CONFIG_PROC_FS
	if6_proc_exit();
proc_if6_fail:
	ipv6_misc_proc_exit();
proc_misc6_fail:
	udplite6_proc_exit();
proc_udplite6_fail:
	raw6_proc_exit();
proc_raw6_fail:
#endif
	ipv6_netfilter_fini();
netfilter_fail:
	igmp6_cleanup();
igmp_fail:
	ndisc_cleanup();
ndisc_fail:
	ip6_mr_cleanup();
ipmr_fail:
	icmpv6_cleanup();
icmp_fail:
	unregister_pernet_subsys(&inet6_net_ops);
register_pernet_fail:
	sock_unregister(PF_INET6);
	rtnl_unregister_all(PF_INET6);
out_sock_register_fail:
	rawv6_exit();
out_unregister_ping_proto:
	proto_unregister(&pingv6_prot);
out_unregister_raw_proto:
	proto_unregister(&rawv6_prot);
out_unregister_udplite_proto:
	proto_unregister(&udplitev6_prot);
out_unregister_udp_proto:
	proto_unregister(&udpv6_prot);
out_unregister_tcp_proto:
	proto_unregister(&tcpv6_prot);
	goto out;
}