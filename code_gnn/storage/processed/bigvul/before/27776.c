int rds_ib_xmit_rdma(struct rds_connection *conn, struct rm_rdma_op *op)
{
	struct rds_ib_connection *ic = conn->c_transport_data;
	struct rds_ib_send_work *send = NULL;
	struct rds_ib_send_work *first;
	struct rds_ib_send_work *prev;
	struct ib_send_wr *failed_wr;
	struct scatterlist *scat;
	unsigned long len;
	u64 remote_addr = op->op_remote_addr;
	u32 max_sge = ic->rds_ibdev->max_sge;
	u32 pos;
	u32 work_alloc;
	u32 i;
	u32 j;
	int sent;
	int ret;
	int num_sge;
	int nr_sig = 0;

	 
	if (!op->op_mapped) {
		op->op_count = ib_dma_map_sg(ic->i_cm_id->device,
					     op->op_sg, op->op_nents, (op->op_write) ?
					     DMA_TO_DEVICE : DMA_FROM_DEVICE);
		rdsdebug("ic %p mapping op %p: %d\n", ic, op, op->op_count);
		if (op->op_count == 0) {
			rds_ib_stats_inc(s_ib_tx_sg_mapping_failure);
			ret = -ENOMEM;  
			goto out;
		}

		op->op_mapped = 1;
	}

	 
	i = ceil(op->op_count, max_sge);

	work_alloc = rds_ib_ring_alloc(&ic->i_send_ring, i, &pos);
	if (work_alloc != i) {
		rds_ib_ring_unalloc(&ic->i_send_ring, work_alloc);
		rds_ib_stats_inc(s_ib_tx_ring_full);
		ret = -ENOMEM;
		goto out;
	}

	send = &ic->i_sends[pos];
	first = send;
	prev = NULL;
	scat = &op->op_sg[0];
	sent = 0;
	num_sge = op->op_count;

	for (i = 0; i < work_alloc && scat != &op->op_sg[op->op_count]; i++) {
		send->s_wr.send_flags = 0;
		send->s_queued = jiffies;
		send->s_op = NULL;

		nr_sig += rds_ib_set_wr_signal_state(ic, send, op->op_notify);

		send->s_wr.opcode = op->op_write ? IB_WR_RDMA_WRITE : IB_WR_RDMA_READ;
		send->s_wr.wr.rdma.remote_addr = remote_addr;
		send->s_wr.wr.rdma.rkey = op->op_rkey;

		if (num_sge > max_sge) {
			send->s_wr.num_sge = max_sge;
			num_sge -= max_sge;
		} else {
			send->s_wr.num_sge = num_sge;
		}

		send->s_wr.next = NULL;

		if (prev)
			prev->s_wr.next = &send->s_wr;

		for (j = 0; j < send->s_wr.num_sge && scat != &op->op_sg[op->op_count]; j++) {
			len = ib_sg_dma_len(ic->i_cm_id->device, scat);
			send->s_sge[j].addr =
				 ib_sg_dma_address(ic->i_cm_id->device, scat);
			send->s_sge[j].length = len;
			send->s_sge[j].lkey = ic->i_mr->lkey;

			sent += len;
			rdsdebug("ic %p sent %d remote_addr %llu\n", ic, sent, remote_addr);

			remote_addr += len;
			scat++;
		}

		rdsdebug("send %p wr %p num_sge %u next %p\n", send,
			&send->s_wr, send->s_wr.num_sge, send->s_wr.next);

		prev = send;
		if (++send == &ic->i_sends[ic->i_send_ring.w_nr])
			send = ic->i_sends;
	}

	 
	if (scat == &op->op_sg[op->op_count]) {
		prev->s_op = op;
		rds_message_addref(container_of(op, struct rds_message, rdma));
	}

	if (i < work_alloc) {
		rds_ib_ring_unalloc(&ic->i_send_ring, work_alloc - i);
		work_alloc = i;
	}

	if (nr_sig)
		atomic_add(nr_sig, &ic->i_signaled_sends);

	failed_wr = &first->s_wr;
	ret = ib_post_send(ic->i_cm_id->qp, &first->s_wr, &failed_wr);
	rdsdebug("ic %p first %p (wr %p) ret %d wr %p\n", ic,
		 first, &first->s_wr, ret, failed_wr);
	BUG_ON(failed_wr != &first->s_wr);
	if (ret) {
		printk(KERN_WARNING "RDS/IB: rdma ib_post_send to %pI4 "
		       "returned %d\n", &conn->c_faddr, ret);
		rds_ib_ring_unalloc(&ic->i_send_ring, work_alloc);
		rds_ib_sub_signaled(ic, nr_sig);
		goto out;
	}

	if (unlikely(failed_wr != &first->s_wr)) {
		printk(KERN_WARNING "RDS/IB: ib_post_send() rc=%d, but failed_wqe updated!\n", ret);
		BUG_ON(failed_wr != &first->s_wr);
	}


out:
	return ret;
}