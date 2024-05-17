static void svc_rdma_wc_receive(struct ib_cq *cq, struct ib_wc *wc)
{
	struct svcxprt_rdma *xprt = cq->cq_context;
	struct ib_cqe *cqe = wc->wr_cqe;
	struct svc_rdma_op_ctxt *ctxt;

	 
	ctxt = container_of(cqe, struct svc_rdma_op_ctxt, cqe);
	svc_rdma_unmap_dma(ctxt);

	if (wc->status != IB_WC_SUCCESS)
		goto flushed;

	 
	ctxt->byte_len = wc->byte_len;
	spin_lock(&xprt->sc_rq_dto_lock);
	list_add_tail(&ctxt->list, &xprt->sc_rq_dto_q);
	spin_unlock(&xprt->sc_rq_dto_lock);

	set_bit(XPT_DATA, &xprt->sc_xprt.xpt_flags);
	if (test_bit(RDMAXPRT_CONN_PENDING, &xprt->sc_flags))
		goto out;
	svc_xprt_enqueue(&xprt->sc_xprt);
	goto out;

flushed:
	if (wc->status != IB_WC_WR_FLUSH_ERR)
		pr_warn("svcrdma: receive: %s (%u/0x%x)\n",
			ib_wc_status_msg(wc->status),
			wc->status, wc->vendor_err);
	set_bit(XPT_CLOSE, &xprt->sc_xprt.xpt_flags);
	svc_rdma_put_context(ctxt, 1);

out:
	svc_xprt_put(&xprt->sc_xprt);
}