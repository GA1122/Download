struct svc_rdma_fastreg_mr *svc_rdma_get_frmr(struct svcxprt_rdma *rdma)
{
	struct svc_rdma_fastreg_mr *frmr = NULL;

	spin_lock(&rdma->sc_frmr_q_lock);
	if (!list_empty(&rdma->sc_frmr_q)) {
		frmr = list_entry(rdma->sc_frmr_q.next,
				  struct svc_rdma_fastreg_mr, frmr_list);
		list_del_init(&frmr->frmr_list);
		frmr->sg_nents = 0;
	}
	spin_unlock(&rdma->sc_frmr_q_lock);
	if (frmr)
		return frmr;

	return rdma_alloc_frmr(rdma);
}
