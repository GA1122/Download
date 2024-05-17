vrrp_dispatcher_read_timeout(sock_t *sock)
{
	vrrp_t *vrrp;
	int prev_state;

	set_time_now();

	rb_for_each_entry_cached(vrrp, &sock->rb_sands, rb_sands) {
		if (vrrp->sands.tv_sec == TIMER_DISABLED ||
		    timercmp(&vrrp->sands, &time_now, >))
			break;

		prev_state = vrrp->state;

		if (vrrp->state == VRRP_STATE_BACK) {
			if (__test_bit(LOG_DETAIL_BIT, &debug))
				log_message(LOG_INFO, "(%s) Receive advertisement timeout", vrrp->iname);
			vrrp_goto_master(vrrp);
		}
		else if (vrrp->state == VRRP_STATE_MAST)
			vrrp_master(vrrp);

		 
#ifdef _TSM_DEBUG_
		if (do_tsm_debug)
			log_message(LOG_INFO, "Send [%s] TSM transition : [%d,%d] Wantstate = [%d]",
				vrrp->iname, prev_state, vrrp->state, vrrp->wantstate);
#endif
		VRRP_TSM_HANDLE(prev_state, vrrp);

		vrrp_init_instance_sands(vrrp);
	}

	return sock->fd_in;
}