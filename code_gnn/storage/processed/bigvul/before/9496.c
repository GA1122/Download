int ntpd_main(int argc UNUSED_PARAM, char **argv)
{
#undef G
	struct globals G;
	struct pollfd *pfd;
	peer_t **idx2peer;
	unsigned cnt;

	memset(&G, 0, sizeof(G));
	SET_PTR_TO_GLOBALS(&G);

	ntp_init(argv);

	 
	cnt = G.peer_cnt + ENABLE_FEATURE_NTPD_SERVER;
	idx2peer = xzalloc(sizeof(idx2peer[0]) * cnt);
	pfd = xzalloc(sizeof(pfd[0]) * cnt);

	 
	cnt = G.peer_cnt * (INITIAL_SAMPLES + 1);

	write_pidfile(CONFIG_PID_FILE_PATH "/ntpd.pid");

	while (!bb_got_signal) {
		llist_t *item;
		unsigned i, j;
		int nfds, timeout;
		double nextaction;

		 

		nextaction = G.cur_time + 3600;

		i = 0;
#if ENABLE_FEATURE_NTPD_SERVER
		if (G_listen_fd != -1) {
			pfd[0].fd = G_listen_fd;
			pfd[0].events = POLLIN;
			i++;
		}
#endif
		 
		for (item = G.ntp_peers; item != NULL; item = item->link) {
			peer_t *p = (peer_t *) item->data;

			if (p->next_action_time <= G.cur_time) {
				if (p->p_fd == -1) {
					 
					if (--cnt == 0) {
						VERB4 bb_error_msg("disabling burst mode");
						G.polladj_count = 0;
						G.poll_exp = MINPOLL;
					}
					send_query_to_peer(p);
				} else {
					 
					close(p->p_fd);
					p->p_fd = -1;
					 
					if (G.poll_exp < BIGPOLL)
						adjust_poll(MINPOLL);
					timeout = poll_interval(NOREPLY_INTERVAL);
					bb_error_msg("timed out waiting for %s, reach 0x%02x, next query in %us",
							p->p_dotted, p->reachable_bits, timeout);

					 
					if (p->reachable_bits == 0)
						resolve_peer_hostname(p,   0);

					set_next(p, timeout);
				}
			}

			if (p->next_action_time < nextaction)
				nextaction = p->next_action_time;

			if (p->p_fd >= 0) {
				 
				pfd[i].fd = p->p_fd;
				pfd[i].events = POLLIN;
				idx2peer[i] = p;
				i++;
			}
		}

		timeout = nextaction - G.cur_time;
		if (timeout < 0)
			timeout = 0;
		timeout++;  

		 
		VERB2 {
			if (i > (ENABLE_FEATURE_NTPD_SERVER && G_listen_fd != -1)) {
				 
				nfds = poll(pfd, i, 1000);
				if (nfds != 0)
					goto did_poll;
				if (--timeout <= 0)
					goto did_poll;
			}
			bb_error_msg("poll:%us sockets:%u interval:%us", timeout, i, 1 << G.poll_exp);
		}
		nfds = poll(pfd, i, timeout * 1000);
 did_poll:
		gettime1900d();  
		if (nfds <= 0) {
			if (!bb_got_signal  
			 && G.cur_time - G.last_script_run > 11*60
			) {
				 
				run_script("periodic", G.last_update_offset);
				gettime1900d();  
			}
			goto check_unsync;
		}

		 
		j = 0;
#if ENABLE_FEATURE_NTPD_SERVER
		if (G.listen_fd != -1) {
			if (pfd[0].revents  ) {
				nfds--;
				recv_and_process_client_pkt( );
				gettime1900d();  
			}
			j = 1;
		}
#endif
		for (; nfds != 0 && j < i; j++) {
			if (pfd[j].revents  ) {
				 
				if (option_mask32 & OPT_qq) {
					option_mask32 &= ~OPT_qq;
					alarm(50);
				}
				nfds--;
				recv_and_process_peer_pkt(idx2peer[j]);
				gettime1900d();  
			}
		}

 check_unsync:
		if (G.ntp_peers && G.stratum != MAXSTRAT) {
			for (item = G.ntp_peers; item != NULL; item = item->link) {
				peer_t *p = (peer_t *) item->data;
				if (p->reachable_bits)
					goto have_reachable_peer;
			}
			 
			clamp_pollexp_and_set_MAXSTRAT();
			run_script("unsync", 0.0);
 have_reachable_peer: ;
		}
	}  

	remove_pidfile(CONFIG_PID_FILE_PATH "/ntpd.pid");
	kill_myself_with_sig(bb_got_signal);
}