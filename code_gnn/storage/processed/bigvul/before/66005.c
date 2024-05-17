int svc_rdma_repost_recv(struct svcxprt_rdma *xprt, gfp_t flags)
{
	int ret = 0;

	ret = svc_rdma_post_recv(xprt, flags);
	if (ret) {
		pr_err("svcrdma: could not post a receive buffer, err=%d.\n",
		       ret);
		pr_err("svcrdma: closing transport %p.\n", xprt);
		set_bit(XPT_CLOSE, &xprt->sc_xprt.xpt_flags);
		ret = -ENOTCONN;
	}
	return ret;
}
