static void svc_rdma_destroy_ctxts(struct svcxprt_rdma *xprt)
{
	while (!list_empty(&xprt->sc_ctxts)) {
		struct svc_rdma_op_ctxt *ctxt;

		ctxt = list_first_entry(&xprt->sc_ctxts,
					struct svc_rdma_op_ctxt, list);
		list_del(&ctxt->list);
		kfree(ctxt);
 	}
 }