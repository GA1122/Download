vrrp_init_state(list l)
{
	vrrp_t *vrrp;
	vrrp_sgroup_t *vgroup;
	element e;
	bool is_up;
	int new_state;

	 
	set_time_now();

	 
	for (e = LIST_HEAD(vrrp_data->vrrp_sync_group); e; ELEMENT_NEXT(e)) {
		 
		vgroup = ELEMENT_DATA(e);

		if (vgroup->state == VRRP_STATE_FAULT)
			send_group_notifies(vgroup);
	}

	for (e = LIST_HEAD(l); e; ELEMENT_NEXT(e)) {
		vrrp = ELEMENT_DATA(e);

		 
		if (vrrp->state == VRRP_STATE_FAULT)
			vrrp->wantstate = VRRP_STATE_FAULT;

		new_state = vrrp->sync ? vrrp->sync->state : vrrp->wantstate;

		is_up = VRRP_ISUP(vrrp);

		if (is_up &&
		    new_state == VRRP_STATE_MAST &&
		    !vrrp->num_script_init && (!vrrp->sync || !vrrp->sync->num_member_init) &&
		    (vrrp->base_priority == VRRP_PRIO_OWNER ||
		     vrrp->reload_master) &&
		    vrrp->wantstate == VRRP_STATE_MAST) {
#ifdef _WITH_LVS_
			 
			if (global_data->lvs_syncd.ifname &&
			    global_data->lvs_syncd.vrrp == vrrp)
				ipvs_syncd_cmd(IPVS_STARTDAEMON,
					       &global_data->lvs_syncd,
					       vrrp->state == VRRP_STATE_MAST ? IPVS_MASTER : IPVS_BACKUP,
					       false,
					       false);
#endif
			if (!vrrp->reload_master) {
#ifdef _WITH_SNMP_RFCV3_
				vrrp->stats->next_master_reason = VRRPV3_MASTER_REASON_PREEMPTED;
#endif

				 
				vrrp->state = VRRP_STATE_BACK;
				vrrp->ms_down_timer = 1;
			}

		} else {
			if (new_state == VRRP_STATE_BACK && vrrp->wantstate == VRRP_STATE_MAST)
				vrrp->ms_down_timer = vrrp->master_adver_int + VRRP_TIMER_SKEW_MIN(vrrp);
			else
				vrrp->ms_down_timer = 3 * vrrp->master_adver_int + VRRP_TIMER_SKEW(vrrp);

#ifdef _WITH_SNMP_RFCV3_
			vrrp->stats->next_master_reason = VRRPV3_MASTER_REASON_MASTER_NO_RESPONSE;
#endif

#ifdef _WITH_LVS_
			 
			if (global_data->lvs_syncd.ifname &&
			    global_data->lvs_syncd.vrrp == vrrp)
				ipvs_syncd_cmd(IPVS_STARTDAEMON,
					       &global_data->lvs_syncd,
					       IPVS_BACKUP,
					       false,
					       false);
#endif

			 
			vrrp_restore_interface(vrrp, false, false);
			if (is_up && new_state != VRRP_STATE_FAULT && !vrrp->num_script_init && (!vrrp->sync || !vrrp->sync->num_member_init)) {
				if (is_up) {
					vrrp->state = VRRP_STATE_BACK;
					log_message(LOG_INFO, "(%s) Entering BACKUP STATE (init)", vrrp->iname);
				}
				else {
					vrrp->state = VRRP_STATE_FAULT;
					log_message(LOG_INFO, "(%s) Entering FAULT STATE (init)", vrrp->iname);
				}
				send_instance_notifies(vrrp);
			}
			vrrp->last_transition = timer_now();
		}
#ifdef _WITH_SNMP_RFC_
		vrrp->stats->uptime = timer_now();
#endif
	}
}