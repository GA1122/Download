try_up_instance(vrrp_t *vrrp, bool leaving_init)
{
	int wantstate;

	if (leaving_init) {
		if (vrrp->num_script_if_fault)
			return;
	}
	else if (--vrrp->num_script_if_fault || vrrp->num_script_init)
		return;

	if (vrrp->wantstate == VRRP_STATE_MAST && vrrp->base_priority == VRRP_PRIO_OWNER) {
		vrrp->wantstate = VRRP_STATE_MAST;
#ifdef _WITH_SNMP_RFCV3_
		vrrp->stats->next_master_reason = VRRPV3_MASTER_REASON_PREEMPTED;
#endif
	} else {
		vrrp->wantstate = VRRP_STATE_BACK;
#ifdef _WITH_SNMP_RFCV3_
		vrrp->stats->next_master_reason = VRRPV3_MASTER_REASON_MASTER_NO_RESPONSE;
#endif
	}

	vrrp->master_adver_int = vrrp->adver_int;
	if (vrrp->wantstate == VRRP_STATE_MAST && vrrp->base_priority == VRRP_PRIO_OWNER)
		vrrp->ms_down_timer = vrrp->master_adver_int + VRRP_TIMER_SKEW(vrrp);
	else
		vrrp->ms_down_timer = 3 * vrrp->master_adver_int + VRRP_TIMER_SKEW(vrrp);

	if (vrrp->sync) {
		if (leaving_init) {
			if (vrrp->sync->num_member_fault)
				return;
		}
		else if (--vrrp->sync->num_member_fault || vrrp->sync->num_member_init)
			return;
	}

	 
	wantstate = vrrp->wantstate;
	if (vrrp->sync && vrrp->wantstate == VRRP_STATE_MAST && !vrrp_sync_can_goto_master(vrrp))
		vrrp->wantstate = VRRP_STATE_BACK;

	 
	vrrp_state_leave_fault(vrrp);

	vrrp_init_instance_sands(vrrp);
	vrrp_thread_requeue_read(vrrp);

	vrrp->wantstate = wantstate;

	if (vrrp->sync) {
		if (vrrp->state == VRRP_STATE_MAST)
			vrrp_sync_master(vrrp);
		else
			vrrp_sync_backup(vrrp);
	}
}