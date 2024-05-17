vrrp_gratuitous_arp_thread(thread_t * thread)
{
	vrrp_t *vrrp = THREAD_ARG(thread);

	 
	vrrp_send_link_update(vrrp, vrrp->garp_rep);

	return 0;
}
