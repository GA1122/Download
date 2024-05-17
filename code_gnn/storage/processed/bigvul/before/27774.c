int rds_ib_xmit_atomic(struct rds_connection *conn, struct rm_atomic_op *op)
{
	struct rds_ib_connection *ic = conn->c_transport_data;
	struct rds_ib_send_work *send = NULL;
	struct ib_send_wr *failed_wr;
	struct rds_ib_device *rds_ibdev;
	u32 pos;
	u32 work_alloc;
	int ret;
	int nr_sig = 0;

	rds_ibdev = ib_get_client_data(ic->i_cm_id->device, &rds_ib_client);

	work_alloc = rds_ib_ring_alloc(&ic->i_send_ring, 1, &pos);
	if (work_alloc != 1) {
		rds_ib_ring_unalloc(&ic->i_send_ring, work_alloc);
		rds_ib_stats_inc(s_ib_tx_ring_full);
		ret = -ENOMEM;
		goto out;
	}

	 
	send = &ic->i_sends[pos];
	send->s_queued = jiffies;

	if (op->op_type == RDS_ATOMIC_TYPE_CSWP) {
		send->s_wr.opcode = IB_WR_MASKED_ATOMIC_CMP_AND_SWP;
		send->s_wr.wr.atomic.compare_add = op->op_m_cswp.compare;
		send->s_wr.wr.atomic.swap = op->op_m_cswp.swap;
		send->s_wr.wr.atomic.compare_add_mask = op->op_m_cswp.compare_mask;
		send->s_wr.wr.atomic.swap_mask = op->op_m_cswp.swap_mask;
	} else {  
		send->s_wr.opcode = IB_WR_MASKED_ATOMIC_FETCH_AND_ADD;
		send->s_wr.wr.atomic.compare_add = op->op_m_fadd.add;
		send->s_wr.wr.atomic.swap = 0;
		send->s_wr.wr.atomic.compare_add_mask = op->op_m_fadd.nocarry_mask;
		send->s_wr.wr.atomic.swap_mask = 0;
	}
	nr_sig = rds_ib_set_wr_signal_state(ic, send, op->op_notify);
	send->s_wr.num_sge = 1;
	send->s_wr.next = NULL;
	send->s_wr.wr.atomic.remote_addr = op->op_remote_addr;
	send->s_wr.wr.atomic.rkey = op->op_rkey;
	send->s_op = op;
	rds_message_addref(container_of(send->s_op, struct rds_message, atomic));

	 
	ret = ib_dma_map_sg(ic->i_cm_id->device, op->op_sg, 1, DMA_FROM_DEVICE);
	rdsdebug("ic %p mapping atomic op %p. mapped %d pg\n", ic, op, ret);
	if (ret != 1) {
		rds_ib_ring_unalloc(&ic->i_send_ring, work_alloc);
		rds_ib_stats_inc(s_ib_tx_sg_mapping_failure);
		ret = -ENOMEM;  
		goto out;
	}

	 
	send->s_sge[0].addr = ib_sg_dma_address(ic->i_cm_id->device, op->op_sg);
	send->s_sge[0].length = ib_sg_dma_len(ic->i_cm_id->device, op->op_sg);
	send->s_sge[0].lkey = ic->i_mr->lkey;

	rdsdebug("rva %Lx rpa %Lx len %u\n", op->op_remote_addr,
		 send->s_sge[0].addr, send->s_sge[0].length);

	if (nr_sig)
		atomic_add(nr_sig, &ic->i_signaled_sends);

	failed_wr = &send->s_wr;
	ret = ib_post_send(ic->i_cm_id->qp, &send->s_wr, &failed_wr);
	rdsdebug("ic %p send %p (wr %p) ret %d wr %p\n", ic,
		 send, &send->s_wr, ret, failed_wr);
	BUG_ON(failed_wr != &send->s_wr);
	if (ret) {
		printk(KERN_WARNING "RDS/IB: atomic ib_post_send to %pI4 "
		       "returned %d\n", &conn->c_faddr, ret);
		rds_ib_ring_unalloc(&ic->i_send_ring, work_alloc);
		rds_ib_sub_signaled(ic, nr_sig);
		goto out;
	}

	if (unlikely(failed_wr != &send->s_wr)) {
		printk(KERN_WARNING "RDS/IB: atomic ib_post_send() rc=%d, but failed_wqe updated!\n", ret);
		BUG_ON(failed_wr != &send->s_wr);
	}

out:
	return ret;
}