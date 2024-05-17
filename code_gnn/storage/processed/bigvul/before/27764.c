void rds_ib_send_cq_comp_handler(struct ib_cq *cq, void *context)
{
	struct rds_connection *conn = context;
	struct rds_ib_connection *ic = conn->c_transport_data;
	struct rds_message *rm = NULL;
	struct ib_wc wc;
	struct rds_ib_send_work *send;
	u32 completed;
	u32 oldest;
	u32 i = 0;
	int ret;
	int nr_sig = 0;

	rdsdebug("cq %p conn %p\n", cq, conn);
	rds_ib_stats_inc(s_ib_tx_cq_call);
	ret = ib_req_notify_cq(cq, IB_CQ_NEXT_COMP);
	if (ret)
		rdsdebug("ib_req_notify_cq send failed: %d\n", ret);

	while (ib_poll_cq(cq, 1, &wc) > 0) {
		rdsdebug("wc wr_id 0x%llx status %u (%s) byte_len %u imm_data %u\n",
			 (unsigned long long)wc.wr_id, wc.status,
			 rds_ib_wc_status_str(wc.status), wc.byte_len,
			 be32_to_cpu(wc.ex.imm_data));
		rds_ib_stats_inc(s_ib_tx_cq_event);

		if (wc.wr_id == RDS_IB_ACK_WR_ID) {
			if (ic->i_ack_queued + HZ/2 < jiffies)
				rds_ib_stats_inc(s_ib_tx_stalled);
			rds_ib_ack_send_complete(ic);
			continue;
		}

		oldest = rds_ib_ring_oldest(&ic->i_send_ring);

		completed = rds_ib_ring_completed(&ic->i_send_ring, wc.wr_id, oldest);

		for (i = 0; i < completed; i++) {
			send = &ic->i_sends[oldest];
			if (send->s_wr.send_flags & IB_SEND_SIGNALED)
				nr_sig++;

			rm = rds_ib_send_unmap_op(ic, send, wc.status);

			if (send->s_queued + HZ/2 < jiffies)
				rds_ib_stats_inc(s_ib_tx_stalled);

			if (send->s_op) {
				if (send->s_op == rm->m_final_op) {
					 
					rds_message_unmapped(rm);
				}
				rds_message_put(rm);
				send->s_op = NULL;
			}

			oldest = (oldest + 1) % ic->i_send_ring.w_nr;
		}

		rds_ib_ring_free(&ic->i_send_ring, completed);
		rds_ib_sub_signaled(ic, nr_sig);
		nr_sig = 0;

		if (test_and_clear_bit(RDS_LL_SEND_FULL, &conn->c_flags) ||
		    test_bit(0, &conn->c_map_queued))
			queue_delayed_work(rds_wq, &conn->c_send_w, 0);

		 
		if (wc.status != IB_WC_SUCCESS && rds_conn_up(conn)) {
			rds_ib_conn_error(conn, "send completion on %pI4 had status "
					  "%u (%s), disconnecting and reconnecting\n",
					  &conn->c_faddr, wc.status,
					  rds_ib_wc_status_str(wc.status));
		}
	}
}