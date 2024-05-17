static void rdma_read_complete(struct svc_rqst *rqstp,
			       struct svc_rdma_op_ctxt *head)
{
	int page_no;

	 
	for (page_no = 0; page_no < head->count; page_no++) {
		put_page(rqstp->rq_pages[page_no]);
		rqstp->rq_pages[page_no] = head->pages[page_no];
	}

	 
	if (head->position == 0) {
		if (head->arg.len <= head->sge[0].length) {
			head->arg.head[0].iov_len = head->arg.len -
							head->byte_len;
			head->arg.page_len = 0;
		} else {
			head->arg.head[0].iov_len = head->sge[0].length -
								head->byte_len;
			head->arg.page_len = head->arg.len -
						head->sge[0].length;
		}
	}

	 
	rqstp->rq_arg.pages = &rqstp->rq_pages[head->hdr_count];
	rqstp->rq_arg.page_len = head->arg.page_len;
	rqstp->rq_arg.page_base = head->arg.page_base;

	 
	rqstp->rq_respages = &rqstp->rq_pages[page_no];
	rqstp->rq_next_page = rqstp->rq_respages + 1;

	 
	rqstp->rq_arg.head[0] = head->arg.head[0];
	rqstp->rq_arg.tail[0] = head->arg.tail[0];
	rqstp->rq_arg.len = head->arg.len;
 	rqstp->rq_arg.buflen = head->arg.buflen;
 }
