svc_init_buffer(struct svc_rqst *rqstp, unsigned int size, int node)
{
	unsigned int pages, arghi;

	 
	if (svc_is_backchannel(rqstp))
		return 1;

	pages = size / PAGE_SIZE + 1;  
	arghi = 0;
	WARN_ON_ONCE(pages > RPCSVC_MAXPAGES);
	if (pages > RPCSVC_MAXPAGES)
		pages = RPCSVC_MAXPAGES;
	while (pages) {
		struct page *p = alloc_pages_node(node, GFP_KERNEL, 0);
		if (!p)
			break;
		rqstp->rq_pages[arghi++] = p;
		pages--;
	}
	return pages == 0;
}
