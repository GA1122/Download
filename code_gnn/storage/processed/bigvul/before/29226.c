static void __ip_vs_del_service(struct ip_vs_service *svc)
{
	struct ip_vs_dest *dest, *nxt;
	struct ip_vs_scheduler *old_sched;

	 
	if (svc->af == AF_INET)
		ip_vs_num_services--;

	ip_vs_kill_estimator(&svc->stats);

	 
	old_sched = svc->scheduler;
	ip_vs_unbind_scheduler(svc);
	if (old_sched)
		ip_vs_scheduler_put(old_sched);

	 
	if (svc->inc) {
		ip_vs_app_inc_put(svc->inc);
		svc->inc = NULL;
	}

	 
	list_for_each_entry_safe(dest, nxt, &svc->destinations, n_list) {
		__ip_vs_unlink_dest(svc, dest, 0);
		__ip_vs_del_dest(dest);
	}

	 
	if (svc->port == FTPPORT)
		atomic_dec(&ip_vs_ftpsvc_counter);
	else if (svc->port == 0)
		atomic_dec(&ip_vs_nullsvc_counter);

	 
	if (atomic_read(&svc->refcnt) == 0)
		kfree(svc);

	 
	ip_vs_use_count_dec();
}