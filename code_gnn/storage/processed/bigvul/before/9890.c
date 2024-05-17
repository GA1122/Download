static void stream_int_update_embedded(struct stream_interface *si)
{
	int old_flags = si->flags;

	DPRINTF(stderr, "%s: si=%p, si->state=%d ib->flags=%08x ob->flags=%08x\n",
		__FUNCTION__,
		si, si->state, si->ib->flags, si->ob->flags);

	if (si->state != SI_ST_EST)
		return;

	if ((si->ob->flags & (CF_SHUTW|CF_SHUTW_NOW)) == CF_SHUTW_NOW &&
	    channel_is_empty(si->ob))
		si_shutw(si);

	if ((si->ob->flags & (CF_SHUTW|CF_SHUTW_NOW)) == 0 && !channel_full(si->ob))
		si->flags |= SI_FL_WAIT_DATA;

	 
	if ((si->ib->flags & (CF_SHUTR|CF_DONT_READ)) == 0 && !channel_is_empty(si->ib))
		si->flags |= SI_FL_WAIT_ROOM;

	if (si->ob->flags & CF_WRITE_ACTIVITY) {
		if (tick_isset(si->ob->wex))
			si->ob->wex = tick_add_ifset(now_ms, si->ob->wto);
	}

	if (si->ib->flags & CF_READ_ACTIVITY ||
	    (si->ob->flags & CF_WRITE_ACTIVITY && !(si->flags & SI_FL_INDEP_STR))) {
		if (tick_isset(si->ib->rex))
			si->ib->rex = tick_add_ifset(now_ms, si->ib->rto);
	}

	 
	old_flags = si->flags;
	if (likely((si->ob->flags & (CF_SHUTW|CF_WRITE_PARTIAL|CF_DONT_READ)) == CF_WRITE_PARTIAL &&
		   !channel_full(si->ob) &&
		   (si->ob->prod->flags & SI_FL_WAIT_ROOM)))
		si_chk_rcv(si->ob->prod);

	if (((si->ib->flags & CF_READ_PARTIAL) && !channel_is_empty(si->ib)) &&
	    (si->ib->cons->flags & SI_FL_WAIT_DATA)) {
		si_chk_snd(si->ib->cons);
		 
		if (!channel_full(si->ib))
			si->flags &= ~SI_FL_WAIT_ROOM;
	}

	 
	if ( 
	    ((old_flags & ~si->flags) & (SI_FL_WAIT_ROOM|SI_FL_WAIT_DATA)) ||

	     
	    (si->ib->flags & (CF_READ_NULL|CF_READ_ERROR)) ||
	    si->state != SI_ST_EST ||
	    (si->flags & SI_FL_ERR) ||
	    ((si->ib->flags & CF_READ_PARTIAL) &&
	     (!si->ib->to_forward || si->ib->cons->state != SI_ST_EST)) ||

	     
	    (si->ob->flags & (CF_WRITE_NULL|CF_WRITE_ERROR)) ||
	    ((si->ob->flags & CF_WRITE_ACTIVITY) &&
	     ((si->ob->flags & CF_SHUTW) ||
	      ((si->ob->flags & CF_WAKE_WRITE) &&
	       (si->ob->prod->state != SI_ST_EST ||
	        (channel_is_empty(si->ob) && !si->ob->to_forward)))))) {
		if (!(si->flags & SI_FL_DONT_WAKE) && si->owner)
			task_wakeup(si->owner, TASK_WOKEN_IO);
	}
	if (si->ib->flags & CF_READ_ACTIVITY)
		si->ib->flags &= ~CF_READ_DONTWAIT;
}