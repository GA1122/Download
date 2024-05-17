static int lockd_inetaddr_event(struct notifier_block *this,
	unsigned long event, void *ptr)
{
	struct in_ifaddr *ifa = (struct in_ifaddr *)ptr;
	struct sockaddr_in sin;

	if (event != NETDEV_DOWN)
		goto out;

	if (nlmsvc_rqst) {
		dprintk("lockd_inetaddr_event: removed %pI4\n",
			&ifa->ifa_local);
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = ifa->ifa_local;
		svc_age_temp_xprts_now(nlmsvc_rqst->rq_server,
			(struct sockaddr *)&sin);
	}

out:
	return NOTIFY_DONE;
}
