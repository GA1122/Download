 int svc_rdma_init(void)
 {
  	dprintk("SVCRDMA Module Init, register RPC RDMA transport\n");
  	dprintk("\tsvcrdma_ord      : %d\n", svcrdma_ord);
  	dprintk("\tmax_requests     : %u\n", svcrdma_max_requests);
	dprintk("\tsq_depth         : %u\n",
		svcrdma_max_requests * RPCRDMA_SQ_DEPTH_MULT);
  	dprintk("\tmax_bc_requests  : %u\n", svcrdma_max_bc_requests);
  	dprintk("\tmax_inline       : %d\n", svcrdma_max_req_size);
  
 	svc_rdma_wq = alloc_workqueue("svc_rdma", 0, 0);
 	if (!svc_rdma_wq)
 		return -ENOMEM;
 
 	if (!svcrdma_table_header)
 		svcrdma_table_header =
 			register_sysctl_table(svcrdma_root_table);
 
 	 
 	svc_reg_xprt_class(&svc_rdma_class);
 #if defined(CONFIG_SUNRPC_BACKCHANNEL)
 	svc_reg_xprt_class(&svc_rdma_bc_class);
 #endif
 	return 0;
 }