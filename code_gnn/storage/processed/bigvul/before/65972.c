rdma_copy_tail(struct svc_rqst *rqstp, struct svc_rdma_op_ctxt *head,
	       u32 position, u32 byte_count, u32 page_offset, int page_no)
{
	char *srcp, *destp;

	srcp = head->arg.head[0].iov_base + position;
	byte_count = head->arg.head[0].iov_len - position;
	if (byte_count > PAGE_SIZE) {
		dprintk("svcrdma: large tail unsupported\n");
		return 0;
	}

	 
	if (page_offset != PAGE_SIZE) {
		destp = page_address(rqstp->rq_arg.pages[page_no]);
		destp += page_offset;
		while (byte_count--) {
			*destp++ = *srcp++;
			page_offset++;
			if (page_offset == PAGE_SIZE && byte_count)
				goto more;
		}
		goto done;
	}

more:
	 
	page_no++;
	destp = page_address(rqstp->rq_arg.pages[page_no]);
	while (byte_count--)
		*destp++ = *srcp++;

	rqstp->rq_respages = &rqstp->rq_arg.pages[page_no+1];
	rqstp->rq_next_page = rqstp->rq_respages + 1;

done:
	byte_count = head->arg.head[0].iov_len - position;
	head->arg.page_len += byte_count;
	head->arg.len += byte_count;
	head->arg.buflen += byte_count;
	return 1;
}