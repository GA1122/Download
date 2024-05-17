static void stream_int_shutw_conn(struct stream_interface *si)
{
	struct connection *conn = __objt_conn(si->end);

	si->ob->flags &= ~CF_SHUTW_NOW;
	if (si->ob->flags & CF_SHUTW)
		return;
	si->ob->flags |= CF_SHUTW;
	si->ob->wex = TICK_ETERNITY;
	si->flags &= ~SI_FL_WAIT_DATA;

	switch (si->state) {
	case SI_ST_EST:
		 
		if (si->flags & SI_FL_ERR) {
			 
		}
		else if (si->flags & SI_FL_NOLINGER) {
			 
			if (conn->xprt && conn->xprt->shutw)
				conn->xprt->shutw(conn, 0);
		}
		else {
			 
			if (conn->xprt && conn->xprt->shutw)
				conn->xprt->shutw(conn, 1);

			 
			if (!(si->flags & SI_FL_NOHALF) || !(si->ib->flags & (CF_SHUTR|CF_DONT_READ))) {
				 
				if (conn_ctrl_ready(conn))
					shutdown(conn->t.sock.fd, SHUT_WR);

				if (!(si->ib->flags & (CF_SHUTR|CF_DONT_READ))) {
					 
					if (conn->ctrl)
						conn_data_stop_send(conn);
					return;
				}
			}
		}

		 
	case SI_ST_CON:
		 
		conn_full_close(conn);
		 
	case SI_ST_CER:
	case SI_ST_QUE:
	case SI_ST_TAR:
		si->state = SI_ST_DIS;
		 
	default:
		si->flags &= ~(SI_FL_WAIT_ROOM | SI_FL_NOLINGER);
		si->ib->flags &= ~CF_SHUTR_NOW;
		si->ib->flags |= CF_SHUTR;
		si->ib->rex = TICK_ETERNITY;
		si->exp = TICK_ETERNITY;
	}
}
