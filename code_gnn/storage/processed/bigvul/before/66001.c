static bool svc_rdma_prealloc_ctxts(struct svcxprt_rdma *xprt)
{
	unsigned int i;

	 
	i = xprt->sc_sq_depth + xprt->sc_rq_depth;

	while (i--) {
		struct svc_rdma_op_ctxt *ctxt;

		ctxt = alloc_ctxt(xprt, GFP_KERNEL);
		if (!ctxt) {
			dprintk("svcrdma: No memory for RDMA ctxt\n");
			return false;
		}
		list_add(&ctxt->list, &xprt->sc_ctxts);
	}
	return true;
}
