static int svc_rdma_has_wspace(struct svc_xprt *xprt)
{
	struct svcxprt_rdma *rdma =
		container_of(xprt, struct svcxprt_rdma, sc_xprt);

	 
	if (waitqueue_active(&rdma->sc_send_wait))
		return 0;

	 
	return 1;
}
