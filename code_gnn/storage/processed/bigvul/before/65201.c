static int lockd_inet6addr_event(struct notifier_block *this,
	unsigned long event, void *ptr)
{
	struct inet6_ifaddr *ifa = (struct inet6_ifaddr *)ptr;
	struct sockaddr_in6 sin6;

	if (event != NETDEV_DOWN)
		goto out;

	if (nlmsvc_rqst) {
		dprintk("lockd_inet6addr_event: removed %pI6\n", &ifa->addr);
		sin6.sin6_family = AF_INET6;
		sin6.sin6_addr = ifa->addr;
		if (ipv6_addr_type(&sin6.sin6_addr) & IPV6_ADDR_LINKLOCAL)
			sin6.sin6_scope_id = ifa->idev->dev->ifindex;
		svc_age_temp_xprts_now(nlmsvc_rqst->rq_server,
			(struct sockaddr *)&sin6);
	}

out:
	return NOTIFY_DONE;
}
