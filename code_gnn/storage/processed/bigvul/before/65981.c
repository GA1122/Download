static struct svc_rdma_op_ctxt *alloc_ctxt(struct svcxprt_rdma *xprt,
					   gfp_t flags)
{
	struct svc_rdma_op_ctxt *ctxt;

	ctxt = kmalloc(sizeof(*ctxt), flags);
	if (ctxt) {
		ctxt->xprt = xprt;
		INIT_LIST_HEAD(&ctxt->list);
	}
	return ctxt;
}
