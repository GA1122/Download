static void stream_int_chk_snd_conn(struct stream_interface *si)
{
	struct channel *ob = si->ob;
	struct connection *conn = __objt_conn(si->end);

	if (unlikely(si->state > SI_ST_EST || (ob->flags & CF_SHUTW)))
		return;

	if (unlikely(channel_is_empty(ob)))   
		return;

	if (!ob->pipe &&                           
	    !(si->flags & SI_FL_WAIT_DATA))        
		return;

	if (conn->flags & (CO_FL_DATA_WR_ENA|CO_FL_CURR_WR_ENA)) {
		 
		return;
	}

	 
	conn_refresh_polling_flags(conn);
	__conn_data_want_send(conn);

	if (!(conn->flags & (CO_FL_HANDSHAKE|CO_FL_WAIT_L4_CONN|CO_FL_WAIT_L6_CONN))) {
		si_conn_send(conn);
		if (conn->flags & CO_FL_ERROR) {
			 
			__conn_data_stop_both(conn);
			si->flags |= SI_FL_ERR;
			goto out_wakeup;
		}
	}

	 
	if (channel_is_empty(ob)) {
		 
		__conn_data_stop_send(conn);
		if (((ob->flags & (CF_SHUTW|CF_AUTO_CLOSE|CF_SHUTW_NOW)) ==
		     (CF_AUTO_CLOSE|CF_SHUTW_NOW)) &&
		    (si->state == SI_ST_EST)) {
			si_shutw(si);
			goto out_wakeup;
		}

		if ((ob->flags & (CF_SHUTW|CF_SHUTW_NOW)) == 0)
			si->flags |= SI_FL_WAIT_DATA;
		ob->wex = TICK_ETERNITY;
	}
	else {
		 
		__conn_data_want_send(conn);
		si->flags &= ~SI_FL_WAIT_DATA;
		if (!tick_isset(ob->wex))
			ob->wex = tick_add_ifset(now_ms, ob->wto);
	}

	if (likely(ob->flags & CF_WRITE_ACTIVITY)) {
		 
		if ((ob->flags & (CF_SHUTW|CF_WRITE_PARTIAL)) == CF_WRITE_PARTIAL &&
		    !channel_is_empty(ob))
			ob->wex = tick_add_ifset(now_ms, ob->wto);

		if (tick_isset(si->ib->rex) && !(si->flags & SI_FL_INDEP_STR)) {
			 
			si->ib->rex = tick_add_ifset(now_ms, si->ib->rto);
		}
	}

	 
	if (likely((ob->flags & (CF_WRITE_NULL|CF_WRITE_ERROR|CF_SHUTW)) ||
	          ((ob->flags & CF_WAKE_WRITE) &&
	           ((channel_is_empty(si->ob) && !ob->to_forward) ||
	            si->state != SI_ST_EST)))) {
	out_wakeup:
		if (!(si->flags & SI_FL_DONT_WAKE) && si->owner)
			task_wakeup(si->owner, TASK_WOKEN_IO);
	}

	 
	conn_cond_update_polling(conn);
}
