static int ipv4_sysctl_rtcache_flush(ctl_table *__ctl, int write,
					void __user *buffer,
					size_t *lenp, loff_t *ppos)
{
	if (write) {
		int flush_delay;
		ctl_table ctl;
		struct net *net;

		memcpy(&ctl, __ctl, sizeof(ctl));
		ctl.data = &flush_delay;
		proc_dointvec(&ctl, write, buffer, lenp, ppos);

		net = (struct net *)__ctl->extra1;
		rt_cache_flush(net, flush_delay);
		return 0;
	}

	return -EINVAL;
}
