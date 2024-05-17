struct svc_rdma_op_ctxt *svc_rdma_get_context(struct svcxprt_rdma *xprt)
{
	struct svc_rdma_op_ctxt *ctxt = NULL;

	spin_lock(&xprt->sc_ctxt_lock);
	xprt->sc_ctxt_used++;
	if (list_empty(&xprt->sc_ctxts))
		goto out_empty;

	ctxt = list_first_entry(&xprt->sc_ctxts,
				struct svc_rdma_op_ctxt, list);
	list_del(&ctxt->list);
	spin_unlock(&xprt->sc_ctxt_lock);

out:
	ctxt->count = 0;
	ctxt->mapped_sges = 0;
	ctxt->frmr = NULL;
	return ctxt;

out_empty:
	 
	spin_unlock(&xprt->sc_ctxt_lock);

	ctxt = alloc_ctxt(xprt, GFP_NOIO);
	if (ctxt)
		goto out;

	spin_lock(&xprt->sc_ctxt_lock);
	xprt->sc_ctxt_used--;
	spin_unlock(&xprt->sc_ctxt_lock);
	WARN_ONCE(1, "svcrdma: empty RDMA ctxt list?\n");
	return NULL;
}
