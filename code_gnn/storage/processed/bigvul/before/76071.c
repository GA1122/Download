vrrp_compute_timer(const sock_t *sock)
{
	vrrp_t *vrrp;
	static timeval_t timer = { .tv_sec = TIMER_DISABLED };

	 
	vrrp = rb_entry(rb_first_cached(&sock->rb_sands), vrrp_t, rb_sands);
	if (vrrp)
		return &vrrp->sands;

	return &timer;
}
