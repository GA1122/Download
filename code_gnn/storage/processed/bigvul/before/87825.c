void rds_recv_incoming(struct rds_connection *conn, struct in6_addr *saddr,
		       struct in6_addr *daddr,
		       struct rds_incoming *inc, gfp_t gfp)
{
	struct rds_sock *rs = NULL;
	struct sock *sk;
	unsigned long flags;
	struct rds_conn_path *cp;

	inc->i_conn = conn;
	inc->i_rx_jiffies = jiffies;
	if (conn->c_trans->t_mp_capable)
		cp = inc->i_conn_path;
	else
		cp = &conn->c_path[0];

	rdsdebug("conn %p next %llu inc %p seq %llu len %u sport %u dport %u "
		 "flags 0x%x rx_jiffies %lu\n", conn,
		 (unsigned long long)cp->cp_next_rx_seq,
		 inc,
		 (unsigned long long)be64_to_cpu(inc->i_hdr.h_sequence),
		 be32_to_cpu(inc->i_hdr.h_len),
		 be16_to_cpu(inc->i_hdr.h_sport),
		 be16_to_cpu(inc->i_hdr.h_dport),
		 inc->i_hdr.h_flags,
		 inc->i_rx_jiffies);

	 
	if (be64_to_cpu(inc->i_hdr.h_sequence) < cp->cp_next_rx_seq &&
	    (inc->i_hdr.h_flags & RDS_FLAG_RETRANSMITTED)) {
		rds_stats_inc(s_recv_drop_old_seq);
		goto out;
	}
	cp->cp_next_rx_seq = be64_to_cpu(inc->i_hdr.h_sequence) + 1;

	if (rds_sysctl_ping_enable && inc->i_hdr.h_dport == 0) {
		if (inc->i_hdr.h_sport == 0) {
			rdsdebug("ignore ping with 0 sport from %pI6c\n",
				 saddr);
			goto out;
		}
		rds_stats_inc(s_recv_ping);
		rds_send_pong(cp, inc->i_hdr.h_sport);
		 
		if (RDS_HS_PROBE(be16_to_cpu(inc->i_hdr.h_sport),
				 be16_to_cpu(inc->i_hdr.h_dport))) {
			rds_recv_hs_exthdrs(&inc->i_hdr, cp->cp_conn);
			rds_start_mprds(cp->cp_conn);
		}
		goto out;
	}

	if (be16_to_cpu(inc->i_hdr.h_dport) ==  RDS_FLAG_PROBE_PORT &&
	    inc->i_hdr.h_sport == 0) {
		rds_recv_hs_exthdrs(&inc->i_hdr, cp->cp_conn);
		 
		rds_start_mprds(cp->cp_conn);
		wake_up(&cp->cp_conn->c_hs_waitq);
		goto out;
	}

	rs = rds_find_bound(daddr, inc->i_hdr.h_dport, conn->c_bound_if);
	if (!rs) {
		rds_stats_inc(s_recv_drop_no_sock);
		goto out;
	}

	 
	rds_recv_incoming_exthdrs(inc, rs);

	 
	sk = rds_rs_to_sk(rs);

	 
	write_lock_irqsave(&rs->rs_recv_lock, flags);
	if (!sock_flag(sk, SOCK_DEAD)) {
		rdsdebug("adding inc %p to rs %p's recv queue\n", inc, rs);
		rds_stats_inc(s_recv_queued);
		rds_recv_rcvbuf_delta(rs, sk, inc->i_conn->c_lcong,
				      be32_to_cpu(inc->i_hdr.h_len),
				      inc->i_hdr.h_dport);
		if (sock_flag(sk, SOCK_RCVTSTAMP))
			inc->i_rx_tstamp = ktime_get_real();
		rds_inc_addref(inc);
		inc->i_rx_lat_trace[RDS_MSG_RX_END] = local_clock();
		list_add_tail(&inc->i_item, &rs->rs_recv_queue);
		__rds_wake_sk_sleep(sk);
	} else {
		rds_stats_inc(s_recv_drop_dead_sock);
	}
	write_unlock_irqrestore(&rs->rs_recv_lock, flags);

out:
	if (rs)
		rds_sock_put(rs);
}
