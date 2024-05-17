vrrp_init_instance_sands(vrrp_t * vrrp)
{
	set_time_now();

	if (vrrp->state == VRRP_STATE_MAST) {
		if (vrrp->reload_master)
			vrrp->sands = time_now;
		else
			vrrp->sands = timer_add_long(time_now, vrrp->adver_int);
	}
	else if (vrrp->state == VRRP_STATE_BACK) {
		 
		vrrp->sands = timer_add_long(time_now, vrrp->ms_down_timer);
	}
	else if (vrrp->state == VRRP_STATE_FAULT || vrrp->state == VRRP_STATE_INIT)
		vrrp->sands.tv_sec = TIMER_DISABLED;

	rb_move_cached(&vrrp->sockets->rb_sands, vrrp, rb_sands, vrrp_timer_cmp);
}