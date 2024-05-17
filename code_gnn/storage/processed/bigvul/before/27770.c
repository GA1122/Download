static void rds_ib_send_unmap_rdma(struct rds_ib_connection *ic,
				   struct rm_rdma_op *op,
				   int wc_status)
{
	if (op->op_mapped) {
		ib_dma_unmap_sg(ic->i_cm_id->device,
				op->op_sg, op->op_nents,
				op->op_write ? DMA_TO_DEVICE : DMA_FROM_DEVICE);
		op->op_mapped = 0;
	}

	 
	rds_ib_send_complete(container_of(op, struct rds_message, rdma),
			     wc_status, rds_rdma_send_complete);

	if (op->op_write)
		rds_stats_add(s_send_rdma_bytes, op->op_bytes);
	else
		rds_stats_add(s_recv_rdma_bytes, op->op_bytes);
}
