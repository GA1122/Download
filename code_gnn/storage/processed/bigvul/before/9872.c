static void si_conn_recv_cb(struct connection *conn)
{
	struct stream_interface *si = conn->owner;
	struct channel *chn = si->ib;
	int ret, max, cur_read;
	int read_poll = MAX_READ_POLL_LOOPS;

	 
	if (conn->flags & CO_FL_ERROR)
		return;

	 
	if (conn_data_read0_pending(conn))
		goto out_shutdown_r;

	 
	if (chn->flags & CF_SHUTR)
		return;

	cur_read = 0;

	if ((chn->flags & (CF_STREAMER | CF_STREAMER_FAST)) && !chn->buf->o &&
	    global.tune.idle_timer &&
	    (unsigned short)(now_ms - chn->last_read) >= global.tune.idle_timer) {
		 
		chn->xfer_small = 0;
		chn->xfer_large = 0;
		chn->flags &= ~(CF_STREAMER | CF_STREAMER_FAST);
	}

	 
	if (conn->xprt->rcv_pipe &&
	    (chn->pipe || chn->to_forward >= MIN_SPLICE_FORWARD) &&
	    chn->flags & CF_KERN_SPLICING) {
		if (buffer_not_empty(chn->buf)) {
			 
			goto abort_splice;
		}

		if (unlikely(chn->pipe == NULL)) {
			if (pipes_used >= global.maxpipes || !(chn->pipe = get_pipe())) {
				chn->flags &= ~CF_KERN_SPLICING;
				goto abort_splice;
			}
		}

		ret = conn->xprt->rcv_pipe(conn, chn->pipe, chn->to_forward);
		if (ret < 0) {
			 
			chn->flags &= ~CF_KERN_SPLICING;
			goto abort_splice;
		}

		if (ret > 0) {
			if (chn->to_forward != CHN_INFINITE_FORWARD)
				chn->to_forward -= ret;
			chn->total += ret;
			cur_read += ret;
			chn->flags |= CF_READ_PARTIAL;
		}

		if (conn_data_read0_pending(conn))
			goto out_shutdown_r;

		if (conn->flags & CO_FL_ERROR)
			return;

		if (conn->flags & CO_FL_WAIT_ROOM) {
			 
			si->flags |= SI_FL_WAIT_ROOM;
			__conn_data_stop_recv(conn);
		}

		 
	}

 abort_splice:
	if (chn->pipe && unlikely(!chn->pipe->data)) {
		put_pipe(chn->pipe);
		chn->pipe = NULL;
	}

	 
	while (!(conn->flags & (CO_FL_ERROR | CO_FL_SOCK_RD_SH | CO_FL_DATA_RD_SH | CO_FL_WAIT_ROOM | CO_FL_HANDSHAKE))) {
		max = bi_avail(chn);

		if (!max) {
			si->flags |= SI_FL_WAIT_ROOM;
			break;
		}

		ret = conn->xprt->rcv_buf(conn, chn->buf, max);
		if (ret <= 0)
			break;

		cur_read += ret;

		 
		if (chn->to_forward && !(chn->flags & (CF_SHUTW|CF_SHUTW_NOW))) {
			unsigned long fwd = ret;
			if (chn->to_forward != CHN_INFINITE_FORWARD) {
				if (fwd > chn->to_forward)
					fwd = chn->to_forward;
				chn->to_forward -= fwd;
			}
			b_adv(chn->buf, fwd);
		}

		chn->flags |= CF_READ_PARTIAL;
		chn->total += ret;

		if (channel_full(chn)) {
			si->flags |= SI_FL_WAIT_ROOM;
			break;
		}

		if ((chn->flags & CF_READ_DONTWAIT) || --read_poll <= 0) {
			si->flags |= SI_FL_WAIT_ROOM;
			__conn_data_stop_recv(conn);
			break;
		}

		 
		if (ret < max) {
			 
			if (chn->flags & CF_STREAMER)
				break;

			 
			if (ret >= global.tune.recv_enough)
				break;
		}
	}  

	if (conn->flags & CO_FL_ERROR)
		return;

	if (cur_read) {
		if ((chn->flags & (CF_STREAMER | CF_STREAMER_FAST)) &&
		    (cur_read <= chn->buf->size / 2)) {
			chn->xfer_large = 0;
			chn->xfer_small++;
			if (chn->xfer_small >= 3) {
				 
				chn->flags &= ~(CF_STREAMER | CF_STREAMER_FAST);
			}
			else if (chn->xfer_small >= 2) {
				 
				chn->flags &= ~CF_STREAMER_FAST;
			}
		}
		else if (!(chn->flags & CF_STREAMER_FAST) &&
			 (cur_read >= chn->buf->size - global.tune.maxrewrite)) {
			 
			chn->xfer_small = 0;
			chn->xfer_large++;
			if (chn->xfer_large >= 3) {
				 
				chn->flags |= (CF_STREAMER | CF_STREAMER_FAST);
			}
		}
		else {
			chn->xfer_small = 0;
			chn->xfer_large = 0;
		}
		chn->last_read = now_ms;
	}

	if (conn_data_read0_pending(conn))
		 
		goto out_shutdown_r;

	return;

 out_shutdown_r:
	 
	chn->flags |= CF_READ_NULL;
	if (chn->flags & CF_AUTO_CLOSE)
		channel_shutw_now(chn);
	stream_sock_read0(si);
	conn_data_read0(conn);
	return;
}
