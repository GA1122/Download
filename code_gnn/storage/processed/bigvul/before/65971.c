static void rdma_build_arg_xdr(struct svc_rqst *rqstp,
			       struct svc_rdma_op_ctxt *ctxt,
			       u32 byte_count)
{
	struct rpcrdma_msg *rmsgp;
	struct page *page;
	u32 bc;
	int sge_no;

	 
	page = ctxt->pages[0];
	put_page(rqstp->rq_pages[0]);
	rqstp->rq_pages[0] = page;

	 
	rqstp->rq_arg.head[0].iov_base = page_address(page);
	rqstp->rq_arg.head[0].iov_len =
		min_t(size_t, byte_count, ctxt->sge[0].length);
	rqstp->rq_arg.len = byte_count;
	rqstp->rq_arg.buflen = byte_count;

	 
	bc = byte_count - rqstp->rq_arg.head[0].iov_len;

	 
	rqstp->rq_arg.page_len = bc;
	rqstp->rq_arg.page_base = 0;

	 
	rmsgp = (struct rpcrdma_msg *)rqstp->rq_arg.head[0].iov_base;
	if (rmsgp->rm_type == rdma_nomsg)
		rqstp->rq_arg.pages = &rqstp->rq_pages[0];
	else
		rqstp->rq_arg.pages = &rqstp->rq_pages[1];

	sge_no = 1;
	while (bc && sge_no < ctxt->count) {
		page = ctxt->pages[sge_no];
		put_page(rqstp->rq_pages[sge_no]);
		rqstp->rq_pages[sge_no] = page;
		bc -= min_t(u32, bc, ctxt->sge[sge_no].length);
		rqstp->rq_arg.buflen += ctxt->sge[sge_no].length;
		sge_no++;
	}
	rqstp->rq_respages = &rqstp->rq_pages[sge_no];
	rqstp->rq_next_page = rqstp->rq_respages + 1;

	 
	bc = sge_no;
	while (sge_no < ctxt->count) {
		page = ctxt->pages[sge_no++];
		put_page(page);
	}
	ctxt->count = bc;

	 
	rqstp->rq_arg.tail[0].iov_base = NULL;
	rqstp->rq_arg.tail[0].iov_len = 0;
}
