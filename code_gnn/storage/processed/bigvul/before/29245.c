ip_vs_del_dest(struct ip_vs_service *svc, struct ip_vs_dest_user_kern *udest)
{
	struct ip_vs_dest *dest;
	__be16 dport = udest->port;

	EnterFunction(2);

	dest = ip_vs_lookup_dest(svc, &udest->addr, dport);

	if (dest == NULL) {
		IP_VS_DBG(1, "%s(): destination not found!\n", __func__);
		return -ENOENT;
	}

	write_lock_bh(&__ip_vs_svc_lock);

	 
	IP_VS_WAIT_WHILE(atomic_read(&svc->usecnt) > 1);

	 
	__ip_vs_unlink_dest(svc, dest, 1);

	write_unlock_bh(&__ip_vs_svc_lock);

	 
	__ip_vs_del_dest(dest);

	LeaveFunction(2);

	return 0;
}