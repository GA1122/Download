static int srpt_perform_rdmas(struct srpt_rdma_ch *ch,
			      struct srpt_send_ioctx *ioctx)
{
	struct ib_send_wr *bad_wr;
	int sq_wr_avail, ret, i;
	enum dma_data_direction dir;
	const int n_rdma = ioctx->n_rdma;

	dir = ioctx->cmd.data_direction;
	if (dir == DMA_TO_DEVICE) {
		 
		ret = -ENOMEM;
		sq_wr_avail = atomic_sub_return(n_rdma, &ch->sq_wr_avail);
		if (sq_wr_avail < 0) {
			pr_warn("IB send queue full (needed %d)\n",
				n_rdma);
			goto out;
		}
	}

	for (i = 0; i < n_rdma; i++) {
		struct ib_send_wr *wr = &ioctx->rdma_wrs[i].wr;

		wr->opcode = (dir == DMA_FROM_DEVICE) ?
				IB_WR_RDMA_WRITE : IB_WR_RDMA_READ;

		if (i == n_rdma - 1) {
			 
			if (dir == DMA_TO_DEVICE) {
				wr->send_flags = IB_SEND_SIGNALED;
				ioctx->rdma_cqe.done = srpt_rdma_read_done;
			} else {
				ioctx->rdma_cqe.done = srpt_rdma_write_done;
			}
			wr->wr_cqe = &ioctx->rdma_cqe;
			wr->next = NULL;
		} else {
			wr->wr_cqe = NULL;
			wr->next = &ioctx->rdma_wrs[i + 1].wr;
		}
	}

	ret = ib_post_send(ch->qp, &ioctx->rdma_wrs->wr, &bad_wr);
	if (ret)
		pr_err("%s[%d]: ib_post_send() returned %d for %d/%d\n",
				 __func__, __LINE__, ret, i, n_rdma);
out:
	if (unlikely(dir == DMA_TO_DEVICE && ret < 0))
		atomic_add(n_rdma, &ch->sq_wr_avail);
	return ret;
}