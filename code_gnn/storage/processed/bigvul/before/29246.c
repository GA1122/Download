static int ip_vs_del_service(struct ip_vs_service *svc)
{
	if (svc == NULL)
		return -EEXIST;

	 
	write_lock_bh(&__ip_vs_svc_lock);

	ip_vs_svc_unhash(svc);

	 
	IP_VS_WAIT_WHILE(atomic_read(&svc->usecnt) > 1);

	__ip_vs_del_service(svc);

	write_unlock_bh(&__ip_vs_svc_lock);

	return 0;
}
