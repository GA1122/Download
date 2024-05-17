static int sctp_inet6_send_verify(struct sctp_sock *opt, union sctp_addr *addr)
{
	struct sctp_af *af = NULL;

	 
	if (addr->sa.sa_family != AF_INET6)
		af = sctp_get_af_specific(addr->sa.sa_family);
	else {
		int type = ipv6_addr_type(&addr->v6.sin6_addr);
		struct net_device *dev;

		if (type & IPV6_ADDR_LINKLOCAL) {
			if (!addr->v6.sin6_scope_id)
				return 0;
			rcu_read_lock();
			dev = dev_get_by_index_rcu(sock_net(&opt->inet.sk),
						   addr->v6.sin6_scope_id);
			rcu_read_unlock();
			if (!dev)
				return 0;
		}
		af = opt->pf->af;
	}

	return af != NULL;
}