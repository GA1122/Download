void svc_rdma_wc_read(struct ib_cq *cq, struct ib_wc *wc)
{
	struct svcxprt_rdma *xprt = cq->cq_context;
	struct ib_cqe *cqe = wc->wr_cqe;
	struct svc_rdma_op_ctxt *ctxt;

	svc_rdma_send_wc_common(xprt, wc, "read");

	ctxt = container_of(cqe, struct svc_rdma_op_ctxt, cqe);
	svc_rdma_unmap_dma(ctxt);
	svc_rdma_put_frmr(xprt, ctxt->frmr);

	if (test_bit(RDMACTXT_F_LAST_CTXT, &ctxt->flags)) {
		struct svc_rdma_op_ctxt *read_hdr;

		read_hdr = ctxt->read_hdr;
		spin_lock(&xprt->sc_rq_dto_lock);
		list_add_tail(&read_hdr->list,
			      &xprt->sc_read_complete_q);
		spin_unlock(&xprt->sc_rq_dto_lock);

		set_bit(XPT_DATA, &xprt->sc_xprt.xpt_flags);
		svc_xprt_enqueue(&xprt->sc_xprt);
	}

	svc_rdma_put_context(ctxt, 0);
	svc_xprt_put(&xprt->sc_xprt);
}
