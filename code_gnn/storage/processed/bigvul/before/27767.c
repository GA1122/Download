static void rds_ib_send_unmap_atomic(struct rds_ib_connection *ic,
				     struct rm_atomic_op *op,
				     int wc_status)
{
	 
	if (op->op_mapped) {
		ib_dma_unmap_sg(ic->i_cm_id->device, op->op_sg, 1,
				DMA_FROM_DEVICE);
		op->op_mapped = 0;
	}

	rds_ib_send_complete(container_of(op, struct rds_message, atomic),
			     wc_status, rds_atomic_send_complete);

	if (op->op_type == RDS_ATOMIC_TYPE_CSWP)
		rds_ib_stats_inc(s_ib_atomic_cswp);
	else
		rds_ib_stats_inc(s_ib_atomic_fadd);
}