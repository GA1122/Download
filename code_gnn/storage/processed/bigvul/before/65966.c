xprt_setup_rdma_bc(struct xprt_create *args)
{
	struct rpc_xprt *xprt;
	struct rpcrdma_xprt *new_xprt;

	if (args->addrlen > sizeof(xprt->addr)) {
		dprintk("RPC:       %s: address too large\n", __func__);
		return ERR_PTR(-EBADF);
	}

	xprt = xprt_alloc(args->net, sizeof(*new_xprt),
			  RPCRDMA_MAX_BC_REQUESTS,
			  RPCRDMA_MAX_BC_REQUESTS);
	if (!xprt) {
		dprintk("RPC:       %s: couldn't allocate rpc_xprt\n",
			__func__);
		return ERR_PTR(-ENOMEM);
	}

	xprt->timeout = &xprt_rdma_bc_timeout;
	xprt_set_bound(xprt);
	xprt_set_connected(xprt);
	xprt->bind_timeout = RPCRDMA_BIND_TO;
	xprt->reestablish_timeout = RPCRDMA_INIT_REEST_TO;
	xprt->idle_timeout = RPCRDMA_IDLE_DISC_TO;

	xprt->prot = XPRT_TRANSPORT_BC_RDMA;
	xprt->tsh_size = RPCRDMA_HDRLEN_MIN / sizeof(__be32);
	xprt->ops = &xprt_rdma_bc_procs;

	memcpy(&xprt->addr, args->dstaddr, args->addrlen);
	xprt->addrlen = args->addrlen;
	xprt_rdma_format_addresses(xprt, (struct sockaddr *)&xprt->addr);
	xprt->resvport = 0;

	xprt->max_payload = xprt_rdma_max_inline_read;

	new_xprt = rpcx_to_rdmax(xprt);
	new_xprt->rx_buf.rb_bc_max_requests = xprt->max_reqs;

	xprt_get(xprt);
	args->bc_xprt->xpt_bc_xprt = xprt;
	xprt->bc_xprt = args->bc_xprt;

	if (!try_module_get(THIS_MODULE))
		goto out_fail;

	 
	xprt_get(xprt);
	return xprt;

out_fail:
	xprt_rdma_free_addresses(xprt);
	args->bc_xprt->xpt_bc_xprt = NULL;
	args->bc_xprt->xpt_bc_xps = NULL;
	xprt_put(xprt);
	xprt_free(xprt);
	return ERR_PTR(-EINVAL);
}