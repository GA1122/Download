int rdma_read_chunk_lcl(struct svcxprt_rdma *xprt,
			struct svc_rqst *rqstp,
			struct svc_rdma_op_ctxt *head,
			int *page_no,
			u32 *page_offset,
			u32 rs_handle,
			u32 rs_length,
			u64 rs_offset,
			bool last)
{
	struct ib_rdma_wr read_wr;
	int pages_needed = PAGE_ALIGN(*page_offset + rs_length) >> PAGE_SHIFT;
	struct svc_rdma_op_ctxt *ctxt = svc_rdma_get_context(xprt);
	int ret, read, pno;
	u32 pg_off = *page_offset;
	u32 pg_no = *page_no;

	ctxt->direction = DMA_FROM_DEVICE;
	ctxt->read_hdr = head;
	pages_needed = min_t(int, pages_needed, xprt->sc_max_sge_rd);
	read = min_t(int, (pages_needed << PAGE_SHIFT) - *page_offset,
		     rs_length);

	for (pno = 0; pno < pages_needed; pno++) {
		int len = min_t(int, rs_length, PAGE_SIZE - pg_off);

		head->arg.pages[pg_no] = rqstp->rq_arg.pages[pg_no];
		head->arg.page_len += len;

		head->arg.len += len;
		if (!pg_off)
			head->count++;
		rqstp->rq_respages = &rqstp->rq_arg.pages[pg_no+1];
		rqstp->rq_next_page = rqstp->rq_respages + 1;
		ctxt->sge[pno].addr =
			ib_dma_map_page(xprt->sc_cm_id->device,
					head->arg.pages[pg_no], pg_off,
					PAGE_SIZE - pg_off,
					DMA_FROM_DEVICE);
		ret = ib_dma_mapping_error(xprt->sc_cm_id->device,
					   ctxt->sge[pno].addr);
		if (ret)
			goto err;
		svc_rdma_count_mappings(xprt, ctxt);

		ctxt->sge[pno].lkey = xprt->sc_pd->local_dma_lkey;
		ctxt->sge[pno].length = len;
		ctxt->count++;

		 
		pg_off += len;
		if (pg_off == PAGE_SIZE) {
			pg_off = 0;
			pg_no++;
		}
		rs_length -= len;
	}

	if (last && rs_length == 0)
		set_bit(RDMACTXT_F_LAST_CTXT, &ctxt->flags);
	else
		clear_bit(RDMACTXT_F_LAST_CTXT, &ctxt->flags);

	memset(&read_wr, 0, sizeof(read_wr));
	ctxt->cqe.done = svc_rdma_wc_read;
	read_wr.wr.wr_cqe = &ctxt->cqe;
	read_wr.wr.opcode = IB_WR_RDMA_READ;
	read_wr.wr.send_flags = IB_SEND_SIGNALED;
	read_wr.rkey = rs_handle;
	read_wr.remote_addr = rs_offset;
	read_wr.wr.sg_list = ctxt->sge;
	read_wr.wr.num_sge = pages_needed;

	ret = svc_rdma_send(xprt, &read_wr.wr);
	if (ret) {
		pr_err("svcrdma: Error %d posting RDMA_READ\n", ret);
		set_bit(XPT_CLOSE, &xprt->sc_xprt.xpt_flags);
		goto err;
	}

	 
	*page_no = pg_no;
	*page_offset = pg_off;
	ret = read;
	atomic_inc(&rdma_stat_read);
	return ret;
 err:
	svc_rdma_unmap_dma(ctxt);
	svc_rdma_put_context(ctxt, 0);
	return ret;
}