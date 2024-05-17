void svc_rdma_put_frmr(struct svcxprt_rdma *rdma,
		       struct svc_rdma_fastreg_mr *frmr)
{
	if (frmr) {
		ib_dma_unmap_sg(rdma->sc_cm_id->device,
				frmr->sg, frmr->sg_nents, frmr->direction);
		spin_lock(&rdma->sc_frmr_q_lock);
		WARN_ON_ONCE(!list_empty(&frmr->frmr_list));
		list_add(&frmr->frmr_list, &rdma->sc_frmr_q);
		spin_unlock(&rdma->sc_frmr_q_lock);
	}
}
