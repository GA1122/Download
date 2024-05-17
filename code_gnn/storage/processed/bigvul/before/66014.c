void svc_rdma_wc_reg(struct ib_cq *cq, struct ib_wc *wc)
{
	svc_rdma_send_wc_common_put(cq, wc, "fastreg");
}
