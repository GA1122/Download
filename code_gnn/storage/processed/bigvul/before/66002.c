void svc_rdma_put_context(struct svc_rdma_op_ctxt *ctxt, int free_pages)
{
	struct svcxprt_rdma *xprt = ctxt->xprt;
	int i;

	if (free_pages)
		for (i = 0; i < ctxt->count; i++)
			put_page(ctxt->pages[i]);

	spin_lock(&xprt->sc_ctxt_lock);
	xprt->sc_ctxt_used--;
	list_add(&ctxt->list, &xprt->sc_ctxts);
	spin_unlock(&xprt->sc_ctxt_lock);
}
