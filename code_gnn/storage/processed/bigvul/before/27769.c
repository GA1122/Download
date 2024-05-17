static struct rds_message *rds_ib_send_unmap_op(struct rds_ib_connection *ic,
						struct rds_ib_send_work *send,
						int wc_status)
{
	struct rds_message *rm = NULL;

	 
	switch (send->s_wr.opcode) {
	case IB_WR_SEND:
		if (send->s_op) {
			rm = container_of(send->s_op, struct rds_message, data);
			rds_ib_send_unmap_data(ic, send->s_op, wc_status);
		}
		break;
	case IB_WR_RDMA_WRITE:
	case IB_WR_RDMA_READ:
		if (send->s_op) {
			rm = container_of(send->s_op, struct rds_message, rdma);
			rds_ib_send_unmap_rdma(ic, send->s_op, wc_status);
		}
		break;
	case IB_WR_ATOMIC_FETCH_AND_ADD:
	case IB_WR_ATOMIC_CMP_AND_SWP:
		if (send->s_op) {
			rm = container_of(send->s_op, struct rds_message, atomic);
			rds_ib_send_unmap_atomic(ic, send->s_op, wc_status);
		}
		break;
	default:
		if (printk_ratelimit())
			printk(KERN_NOTICE
			       "RDS/IB: %s: unexpected opcode 0x%x in WR!\n",
			       __func__, send->s_wr.opcode);
		break;
	}

	send->s_wr.opcode = 0xdead;

	return rm;
}