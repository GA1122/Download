int svc_rdma_send(struct svcxprt_rdma *xprt, struct ib_send_wr *wr)
{
	struct ib_send_wr *bad_wr, *n_wr;
	int wr_count;
	int i;
	int ret;

	if (test_bit(XPT_CLOSE, &xprt->sc_xprt.xpt_flags))
		return -ENOTCONN;

	wr_count = 1;
	for (n_wr = wr->next; n_wr; n_wr = n_wr->next)
		wr_count++;

	 
	while (1) {
		if ((atomic_sub_return(wr_count, &xprt->sc_sq_avail) < 0)) {
			atomic_inc(&rdma_stat_sq_starve);

			 
			atomic_add(wr_count, &xprt->sc_sq_avail);
			wait_event(xprt->sc_send_wait,
				   atomic_read(&xprt->sc_sq_avail) > wr_count);
			if (test_bit(XPT_CLOSE, &xprt->sc_xprt.xpt_flags))
				return -ENOTCONN;
			continue;
		}
		 
		for (i = 0; i < wr_count; i++)
			svc_xprt_get(&xprt->sc_xprt);

		 
		ret = ib_post_send(xprt->sc_qp, wr, &bad_wr);
		if (ret) {
			set_bit(XPT_CLOSE, &xprt->sc_xprt.xpt_flags);
			for (i = 0; i < wr_count; i ++)
				svc_xprt_put(&xprt->sc_xprt);
			dprintk("svcrdma: failed to post SQ WR rc=%d\n", ret);
			dprintk("    sc_sq_avail=%d, sc_sq_depth=%d\n",
				atomic_read(&xprt->sc_sq_avail),
				xprt->sc_sq_depth);
			wake_up(&xprt->sc_send_wait);
		}
		break;
	}
	return ret;
}