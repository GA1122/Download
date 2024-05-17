static void rdma_dealloc_frmr_q(struct svcxprt_rdma *xprt)
{
	struct svc_rdma_fastreg_mr *frmr;

	while (!list_empty(&xprt->sc_frmr_q)) {
		frmr = list_entry(xprt->sc_frmr_q.next,
				  struct svc_rdma_fastreg_mr, frmr_list);
		list_del_init(&frmr->frmr_list);
		kfree(frmr->sg);
		ib_dereg_mr(frmr->mr);
		kfree(frmr);
	}
}
