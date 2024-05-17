static int rdma_read_chunks(struct svcxprt_rdma *xprt,
			    struct rpcrdma_msg *rmsgp,
			    struct svc_rqst *rqstp,
			    struct svc_rdma_op_ctxt *head)
{
	int page_no, ret;
	struct rpcrdma_read_chunk *ch;
	u32 handle, page_offset, byte_count;
	u32 position;
	u64 rs_offset;
	bool last;

	 
	ch = svc_rdma_get_read_chunk(rmsgp);
	if (!ch)
		return 0;

	if (rdma_rcl_chunk_count(ch) > RPCSVC_MAXPAGES)
		return -EINVAL;

	 
	head->arg.head[0] = rqstp->rq_arg.head[0];
	head->arg.tail[0] = rqstp->rq_arg.tail[0];
	head->hdr_count = head->count;
	head->arg.page_base = 0;
	head->arg.page_len = 0;
	head->arg.len = rqstp->rq_arg.len;
	head->arg.buflen = rqstp->rq_arg.buflen;

	 
	position = be32_to_cpu(ch->rc_position);
	if (position == 0) {
		head->arg.pages = &head->pages[0];
		page_offset = head->byte_len;
	} else {
		head->arg.pages = &head->pages[head->count];
		page_offset = 0;
	}

	ret = 0;
	page_no = 0;
	for (; ch->rc_discrim != xdr_zero; ch++) {
		if (be32_to_cpu(ch->rc_position) != position)
			goto err;

		handle = be32_to_cpu(ch->rc_target.rs_handle),
		byte_count = be32_to_cpu(ch->rc_target.rs_length);
		xdr_decode_hyper((__be32 *)&ch->rc_target.rs_offset,
				 &rs_offset);

		while (byte_count > 0) {
			last = (ch + 1)->rc_discrim == xdr_zero;
			ret = xprt->sc_reader(xprt, rqstp, head,
					      &page_no, &page_offset,
					      handle, byte_count,
					      rs_offset, last);
			if (ret < 0)
				goto err;
			byte_count -= ret;
			rs_offset += ret;
			head->arg.buflen += ret;
		}
	}

	 
	if (page_offset & 3) {
		u32 pad = 4 - (page_offset & 3);

		head->arg.tail[0].iov_len += pad;
		head->arg.len += pad;
		head->arg.buflen += pad;
		page_offset += pad;
	}

	ret = 1;
	if (position && position < head->arg.head[0].iov_len)
		ret = rdma_copy_tail(rqstp, head, position,
				     byte_count, page_offset, page_no);
	head->arg.head[0].iov_len = position;
	head->position = position;

 err:
	 
	for (page_no = 0;
	     &rqstp->rq_pages[page_no] < rqstp->rq_respages; page_no++)
		rqstp->rq_pages[page_no] = NULL;

	return ret;
}