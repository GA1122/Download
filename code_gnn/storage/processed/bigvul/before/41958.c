int rds_send_xmit(struct rds_connection *conn)
{
	struct rds_message *rm;
	unsigned long flags;
	unsigned int tmp;
	struct scatterlist *sg;
	int ret = 0;
	LIST_HEAD(to_be_dropped);
	int batch_count;
	unsigned long send_gen = 0;

restart:
	batch_count = 0;

	 
	if (!acquire_in_xmit(conn)) {
		rds_stats_inc(s_send_lock_contention);
		ret = -ENOMEM;
		goto out;
	}

	 
	conn->c_send_gen++;
	send_gen = conn->c_send_gen;

	 
	if (!rds_conn_up(conn)) {
		release_in_xmit(conn);
		ret = 0;
		goto out;
	}

	if (conn->c_trans->xmit_prepare)
		conn->c_trans->xmit_prepare(conn);

	 
	while (1) {

		rm = conn->c_xmit_rm;

		 
		if (!rm && test_and_clear_bit(0, &conn->c_map_queued)) {
			rm = rds_cong_update_alloc(conn);
			if (IS_ERR(rm)) {
				ret = PTR_ERR(rm);
				break;
			}
			rm->data.op_active = 1;

			conn->c_xmit_rm = rm;
		}

		 
		if (!rm) {
			unsigned int len;

			batch_count++;

			 
			if (batch_count >= send_batch_count)
				goto over_batch;

			spin_lock_irqsave(&conn->c_lock, flags);

			if (!list_empty(&conn->c_send_queue)) {
				rm = list_entry(conn->c_send_queue.next,
						struct rds_message,
						m_conn_item);
				rds_message_addref(rm);

				 
				list_move_tail(&rm->m_conn_item, &conn->c_retrans);
			}

			spin_unlock_irqrestore(&conn->c_lock, flags);

			if (!rm)
				break;

			 
			if (rm->rdma.op_active &&
			    test_bit(RDS_MSG_RETRANSMITTED, &rm->m_flags)) {
				spin_lock_irqsave(&conn->c_lock, flags);
				if (test_and_clear_bit(RDS_MSG_ON_CONN, &rm->m_flags))
					list_move(&rm->m_conn_item, &to_be_dropped);
				spin_unlock_irqrestore(&conn->c_lock, flags);
				continue;
			}

			 
			len = ntohl(rm->m_inc.i_hdr.h_len);
			if (conn->c_unacked_packets == 0 ||
			    conn->c_unacked_bytes < len) {
				__set_bit(RDS_MSG_ACK_REQUIRED, &rm->m_flags);

				conn->c_unacked_packets = rds_sysctl_max_unacked_packets;
				conn->c_unacked_bytes = rds_sysctl_max_unacked_bytes;
				rds_stats_inc(s_send_ack_required);
			} else {
				conn->c_unacked_bytes -= len;
				conn->c_unacked_packets--;
			}

			conn->c_xmit_rm = rm;
		}

		 
		if (rm->rdma.op_active && !conn->c_xmit_rdma_sent) {
			rm->m_final_op = &rm->rdma;
			 
			set_bit(RDS_MSG_MAPPED, &rm->m_flags);
			ret = conn->c_trans->xmit_rdma(conn, &rm->rdma);
			if (ret) {
				clear_bit(RDS_MSG_MAPPED, &rm->m_flags);
				wake_up_interruptible(&rm->m_flush_wait);
				break;
			}
			conn->c_xmit_rdma_sent = 1;

		}

		if (rm->atomic.op_active && !conn->c_xmit_atomic_sent) {
			rm->m_final_op = &rm->atomic;
			 
			set_bit(RDS_MSG_MAPPED, &rm->m_flags);
			ret = conn->c_trans->xmit_atomic(conn, &rm->atomic);
			if (ret) {
				clear_bit(RDS_MSG_MAPPED, &rm->m_flags);
				wake_up_interruptible(&rm->m_flush_wait);
				break;
			}
			conn->c_xmit_atomic_sent = 1;

		}

		 
		if (rm->data.op_nents == 0) {
			int ops_present;
			int all_ops_are_silent = 1;

			ops_present = (rm->atomic.op_active || rm->rdma.op_active);
			if (rm->atomic.op_active && !rm->atomic.op_silent)
				all_ops_are_silent = 0;
			if (rm->rdma.op_active && !rm->rdma.op_silent)
				all_ops_are_silent = 0;

			if (ops_present && all_ops_are_silent
			    && !rm->m_rdma_cookie)
				rm->data.op_active = 0;
		}

		if (rm->data.op_active && !conn->c_xmit_data_sent) {
			rm->m_final_op = &rm->data;
			ret = conn->c_trans->xmit(conn, rm,
						  conn->c_xmit_hdr_off,
						  conn->c_xmit_sg,
						  conn->c_xmit_data_off);
			if (ret <= 0)
				break;

			if (conn->c_xmit_hdr_off < sizeof(struct rds_header)) {
				tmp = min_t(int, ret,
					    sizeof(struct rds_header) -
					    conn->c_xmit_hdr_off);
				conn->c_xmit_hdr_off += tmp;
				ret -= tmp;
			}

			sg = &rm->data.op_sg[conn->c_xmit_sg];
			while (ret) {
				tmp = min_t(int, ret, sg->length -
						      conn->c_xmit_data_off);
				conn->c_xmit_data_off += tmp;
				ret -= tmp;
				if (conn->c_xmit_data_off == sg->length) {
					conn->c_xmit_data_off = 0;
					sg++;
					conn->c_xmit_sg++;
					BUG_ON(ret != 0 &&
					       conn->c_xmit_sg == rm->data.op_nents);
				}
			}

			if (conn->c_xmit_hdr_off == sizeof(struct rds_header) &&
			    (conn->c_xmit_sg == rm->data.op_nents))
				conn->c_xmit_data_sent = 1;
		}

		 
		if (!rm->data.op_active || conn->c_xmit_data_sent) {
			conn->c_xmit_rm = NULL;
			conn->c_xmit_sg = 0;
			conn->c_xmit_hdr_off = 0;
			conn->c_xmit_data_off = 0;
			conn->c_xmit_rdma_sent = 0;
			conn->c_xmit_atomic_sent = 0;
			conn->c_xmit_data_sent = 0;

			rds_message_put(rm);
		}
	}

over_batch:
	if (conn->c_trans->xmit_complete)
		conn->c_trans->xmit_complete(conn);
	release_in_xmit(conn);

	 
	if (!list_empty(&to_be_dropped)) {
		 
		list_for_each_entry(rm, &to_be_dropped, m_conn_item)
			rds_message_put(rm);
		rds_send_remove_from_sock(&to_be_dropped, RDS_RDMA_DROPPED);
	}

	 
	if (ret == 0) {
		smp_mb();
		if ((test_bit(0, &conn->c_map_queued) ||
		     !list_empty(&conn->c_send_queue)) &&
		    send_gen == conn->c_send_gen) {
			rds_stats_inc(s_send_lock_queue_raced);
			if (batch_count < send_batch_count)
				goto restart;
			queue_delayed_work(rds_wq, &conn->c_send_w, 1);
		}
	}
out:
	return ret;
}
