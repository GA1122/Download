__ip_vs_update_dest(struct ip_vs_service *svc, struct ip_vs_dest *dest,
		    struct ip_vs_dest_user_kern *udest, int add)
{
	struct netns_ipvs *ipvs = net_ipvs(svc->net);
	int conn_flags;

	 
	atomic_set(&dest->weight, udest->weight);
	conn_flags = udest->conn_flags & IP_VS_CONN_F_DEST_MASK;
	conn_flags |= IP_VS_CONN_F_INACTIVE;

	 
	if ((conn_flags & IP_VS_CONN_F_FWD_MASK) != IP_VS_CONN_F_MASQ) {
		conn_flags |= IP_VS_CONN_F_NOOUTPUT;
	} else {
		 
		write_lock_bh(&ipvs->rs_lock);
		ip_vs_rs_hash(ipvs, dest);
		write_unlock_bh(&ipvs->rs_lock);
	}
	atomic_set(&dest->conn_flags, conn_flags);

	 
	if (!dest->svc) {
		__ip_vs_bind_svc(dest, svc);
	} else {
		if (dest->svc != svc) {
			__ip_vs_unbind_svc(dest);
			ip_vs_zero_stats(&dest->stats);
			__ip_vs_bind_svc(dest, svc);
		}
	}

	 
	dest->flags |= IP_VS_DEST_F_AVAILABLE;

	if (udest->u_threshold == 0 || udest->u_threshold > dest->u_threshold)
		dest->flags &= ~IP_VS_DEST_F_OVERLOAD;
	dest->u_threshold = udest->u_threshold;
	dest->l_threshold = udest->l_threshold;

	spin_lock_bh(&dest->dst_lock);
	ip_vs_dst_reset(dest);
	spin_unlock_bh(&dest->dst_lock);

	if (add)
		ip_vs_start_estimator(svc->net, &dest->stats);

	write_lock_bh(&__ip_vs_svc_lock);

	 
	IP_VS_WAIT_WHILE(atomic_read(&svc->usecnt) > 0);

	if (add) {
		list_add(&dest->n_list, &svc->destinations);
		svc->num_dests++;
	}

	 
	if (svc->scheduler->update_service)
		svc->scheduler->update_service(svc);

	write_unlock_bh(&__ip_vs_svc_lock);
}
