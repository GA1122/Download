static void svc_rdma_send_wc_common_put(struct ib_cq *cq, struct ib_wc *wc,
					const char *opname)
{
	struct svcxprt_rdma *xprt = cq->cq_context;

	svc_rdma_send_wc_common(xprt, wc, opname);
	svc_xprt_put(&xprt->sc_xprt);
}
