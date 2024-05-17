int rdma_read_chunk_frmr(struct svcxprt_rdma *xprt,
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
	struct ib_send_wr inv_wr;
	struct ib_reg_wr reg_wr;
	u8 key;
	int nents = PAGE_ALIGN(*page_offset + rs_length) >> PAGE_SHIFT;
	struct svc_rdma_op_ctxt *ctxt = svc_rdma_get_context(xprt);
	struct svc_rdma_fastreg_mr *frmr = svc_rdma_get_frmr(xprt);
	int ret, read, pno, dma_nents, n;
	u32 pg_off = *page_offset;
	u32 pg_no = *page_no;

	if (IS_ERR(frmr))
		return -ENOMEM;

	ctxt->direction = DMA_FROM_DEVICE;
	ctxt->frmr = frmr;
	nents = min_t(unsigned int, nents, xprt->sc_frmr_pg_list_len);
	read = min_t(int, (nents << PAGE_SHIFT) - *page_offset, rs_length);

	frmr->direction = DMA_FROM_DEVICE;
	frmr->access_flags = (IB_ACCESS_LOCAL_WRITE|IB_ACCESS_REMOTE_WRITE);
	frmr->sg_nents = nents;

	for (pno = 0; pno < nents; pno++) {
		int len = min_t(int, rs_length, PAGE_SIZE - pg_off);

		head->arg.pages[pg_no] = rqstp->rq_arg.pages[pg_no];
		head->arg.page_len += len;
		head->arg.len += len;
		if (!pg_off)
			head->count++;

		sg_set_page(&frmr->sg[pno], rqstp->rq_arg.pages[pg_no],
			    len, pg_off);

		rqstp->rq_respages = &rqstp->rq_arg.pages[pg_no+1];
		rqstp->rq_next_page = rqstp->rq_respages + 1;

		 
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

	dma_nents = ib_dma_map_sg(xprt->sc_cm_id->device,
				  frmr->sg, frmr->sg_nents,
				  frmr->direction);
	if (!dma_nents) {
		pr_err("svcrdma: failed to dma map sg %p\n",
		       frmr->sg);
		return -ENOMEM;
	}

	n = ib_map_mr_sg(frmr->mr, frmr->sg, frmr->sg_nents, NULL, PAGE_SIZE);
	if (unlikely(n != frmr->sg_nents)) {
		pr_err("svcrdma: failed to map mr %p (%d/%d elements)\n",
		       frmr->mr, n, frmr->sg_nents);
		return n < 0 ? n : -EINVAL;
	}

	 
	key = (u8)(frmr->mr->lkey & 0x000000FF);
	ib_update_fast_reg_key(frmr->mr, ++key);

	ctxt->sge[0].addr = frmr->mr->iova;
	ctxt->sge[0].lkey = frmr->mr->lkey;
	ctxt->sge[0].length = frmr->mr->length;
	ctxt->count = 1;
	ctxt->read_hdr = head;

	 
	ctxt->reg_cqe.done = svc_rdma_wc_reg;
	reg_wr.wr.wr_cqe = &ctxt->reg_cqe;
	reg_wr.wr.opcode = IB_WR_REG_MR;
	reg_wr.wr.send_flags = IB_SEND_SIGNALED;
	reg_wr.wr.num_sge = 0;
	reg_wr.mr = frmr->mr;
	reg_wr.key = frmr->mr->lkey;
	reg_wr.access = frmr->access_flags;
	reg_wr.wr.next = &read_wr.wr;

	 
	memset(&read_wr, 0, sizeof(read_wr));
	ctxt->cqe.done = svc_rdma_wc_read;
	read_wr.wr.wr_cqe = &ctxt->cqe;
	read_wr.wr.send_flags = IB_SEND_SIGNALED;
	read_wr.rkey = rs_handle;
	read_wr.remote_addr = rs_offset;
	read_wr.wr.sg_list = ctxt->sge;
	read_wr.wr.num_sge = 1;
	if (xprt->sc_dev_caps & SVCRDMA_DEVCAP_READ_W_INV) {
		read_wr.wr.opcode = IB_WR_RDMA_READ_WITH_INV;
		read_wr.wr.ex.invalidate_rkey = ctxt->frmr->mr->lkey;
	} else {
		read_wr.wr.opcode = IB_WR_RDMA_READ;
		read_wr.wr.next = &inv_wr;
		 
		memset(&inv_wr, 0, sizeof(inv_wr));
		ctxt->inv_cqe.done = svc_rdma_wc_inv;
		inv_wr.wr_cqe = &ctxt->inv_cqe;
		inv_wr.opcode = IB_WR_LOCAL_INV;
		inv_wr.send_flags = IB_SEND_SIGNALED | IB_SEND_FENCE;
		inv_wr.ex.invalidate_rkey = frmr->mr->lkey;
	}

	 
	ret = svc_rdma_send(xprt, &reg_wr.wr);
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
	svc_rdma_put_context(ctxt, 0);
	svc_rdma_put_frmr(xprt, frmr);
	return ret;
}