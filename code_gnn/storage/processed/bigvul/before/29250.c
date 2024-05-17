static int ip_vs_flush(void)
{
	int idx;
	struct ip_vs_service *svc, *nxt;

	 
	for(idx = 0; idx < IP_VS_SVC_TAB_SIZE; idx++) {
		list_for_each_entry_safe(svc, nxt, &ip_vs_svc_table[idx], s_list) {
			write_lock_bh(&__ip_vs_svc_lock);
			ip_vs_svc_unhash(svc);
			 
			IP_VS_WAIT_WHILE(atomic_read(&svc->usecnt) > 0);
			__ip_vs_del_service(svc);
			write_unlock_bh(&__ip_vs_svc_lock);
		}
	}

	 
	for(idx = 0; idx < IP_VS_SVC_TAB_SIZE; idx++) {
		list_for_each_entry_safe(svc, nxt,
					 &ip_vs_svc_fwm_table[idx], f_list) {
			write_lock_bh(&__ip_vs_svc_lock);
			ip_vs_svc_unhash(svc);
			 
			IP_VS_WAIT_WHILE(atomic_read(&svc->usecnt) > 0);
			__ip_vs_del_service(svc);
			write_unlock_bh(&__ip_vs_svc_lock);
		}
	}

	return 0;
}