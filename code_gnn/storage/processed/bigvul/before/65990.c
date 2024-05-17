static void svc_rdma_bc_free(struct svc_xprt *xprt)
{
	struct svcxprt_rdma *rdma =
		container_of(xprt, struct svcxprt_rdma, sc_xprt);

	dprintk("svcrdma: %s(%p)\n", __func__, xprt);
	if (xprt)
		kfree(rdma);
}
