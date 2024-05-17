vrrp_lower_prio_gratuitous_arp_thread(thread_t * thread)
{
	vrrp_t *vrrp = THREAD_ARG(thread);

	 
	vrrp_send_link_update(vrrp, vrrp->garp_lower_prio_rep);

	return 0;
}
