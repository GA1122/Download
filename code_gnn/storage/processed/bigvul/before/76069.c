vrrp_arp_thread(thread_t *thread)
{
	element e, a;
	list l;
	ip_address_t *ipaddress;
	timeval_t next_time = {
		.tv_sec = INT_MAX	 
	};
	interface_t *ifp;
	vrrp_t *vrrp;
	enum {
		VIP,
		EVIP
	} i;

	set_time_now();

	for (e = LIST_HEAD(vrrp_data->vrrp); e; ELEMENT_NEXT(e)) {
		vrrp = ELEMENT_DATA(e);

		if (!vrrp->garp_pending && !vrrp->gna_pending)
			continue;

		vrrp->garp_pending = false;
		vrrp->gna_pending = false;

		if (vrrp->state != VRRP_STATE_MAST ||
		    !vrrp->vipset)
			continue;

		for (i = VIP; i <= EVIP; i++) {
			l = (i == VIP) ? vrrp->vip : vrrp->evip;

			if (!LIST_ISEMPTY(l)) {
				for (a = LIST_HEAD(l); a; ELEMENT_NEXT(a)) {
					ipaddress = ELEMENT_DATA(a);
					if (!ipaddress->garp_gna_pending)
						continue;
					if (!ipaddress->set) {
						ipaddress->garp_gna_pending = false;
						continue;
					}

					ifp = IF_BASE_IFP(ipaddress->ifp);

					 
					if (!ifp->garp_delay) {
						ipaddress->garp_gna_pending = false;
						continue;
					}

					if (!IP_IS6(ipaddress)) {
						if (timercmp(&time_now, &ifp->garp_delay->garp_next_time, >=)) {
							send_gratuitous_arp_immediate(ifp, ipaddress);
							ipaddress->garp_gna_pending = false;
						}
						else {
							vrrp->garp_pending = true;
							if (timercmp(&ifp->garp_delay->garp_next_time, &next_time, <))
								next_time = ifp->garp_delay->garp_next_time;
						}
					}
					else {
						if (timercmp(&time_now, &ifp->garp_delay->gna_next_time, >=)) {
							ndisc_send_unsolicited_na_immediate(ifp, ipaddress);
							ipaddress->garp_gna_pending = false;
						}
						else {
							vrrp->gna_pending = true;
							if (timercmp(&ifp->garp_delay->gna_next_time, &next_time, <))
								next_time = ifp->garp_delay->gna_next_time;
						}
					}
				}
			}
		}
	}

	if (next_time.tv_sec != INT_MAX) {
		 
		garp_next_time = next_time;

		garp_thread = thread_add_timer(thread->master, vrrp_arp_thread, NULL,
						 timer_long(timer_sub_now(next_time)));
	}
	else
		garp_thread = NULL;

	return 0;
}