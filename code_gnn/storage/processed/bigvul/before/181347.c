  int svc_rdma_sendto(struct svc_rqst *rqstp)
  {
  	struct svc_xprt *xprt = rqstp->rq_xprt;
  	struct svcxprt_rdma *rdma =
  		container_of(xprt, struct svcxprt_rdma, sc_xprt);
	struct rpcrdma_msg *rdma_argp;
	struct rpcrdma_msg *rdma_resp;
	struct rpcrdma_write_array *wr_ary, *rp_ary;
	int ret;
	int inline_bytes;
// 	__be32 *p, *rdma_argp, *rdma_resp, *wr_lst, *rp_ch;
// 	struct xdr_buf *xdr = &rqstp->rq_res;
  	struct page *res_page;
	struct svc_rdma_req_map *vec;
	u32 inv_rkey;
	__be32 *p;
	dprintk("svcrdma: sending response for rqstp=%p\n", rqstp);
// 	int ret;
  
 	 
  	rdma_argp = page_address(rqstp->rq_pages[0]);
	svc_rdma_get_write_arrays(rdma_argp, &wr_ary, &rp_ary);
	inv_rkey = 0;
	if (rdma->sc_snd_w_inv)
		inv_rkey = svc_rdma_get_inv_rkey(rdma_argp, wr_ary, rp_ary);
// 	svc_rdma_get_write_arrays(rdma_argp, &wr_lst, &rp_ch);
  
	 
	vec = svc_rdma_get_req_map(rdma);
	ret = svc_rdma_map_xdr(rdma, &rqstp->rq_res, vec, wr_ary != NULL);
	if (ret)
		goto err0;
	inline_bytes = rqstp->rq_res.len;
// 	dprintk("svcrdma: preparing response for XID 0x%08x\n",
// 		be32_to_cpup(rdma_argp));
  
  	 
 	ret = -ENOMEM;
 	res_page = alloc_page(GFP_KERNEL);
 	if (!res_page)
  		goto err0;
  	rdma_resp = page_address(res_page);
  
	p = &rdma_resp->rm_xid;
	*p++ = rdma_argp->rm_xid;
	*p++ = rdma_argp->rm_vers;
// 	p = rdma_resp;
// 	*p++ = *rdma_argp;
// 	*p++ = *(rdma_argp + 1);
  	*p++ = rdma->sc_fc_credits;
	*p++ = rp_ary ? rdma_nomsg : rdma_msg;
// 	*p++ = rp_ch ? rdma_nomsg : rdma_msg;
  
  	 
  	*p++ = xdr_zero;
  	*p++ = xdr_zero;
  	*p   = xdr_zero;
  
	 
	if (wr_ary) {
		ret = send_write_chunks(rdma, wr_ary, rdma_resp, rqstp, vec);
// 	if (wr_lst) {
// 		 
// 		ret = svc_rdma_send_write_chunk(rdma, wr_lst, xdr);
  		if (ret < 0)
			goto err1;
		inline_bytes -= ret + xdr_padsize(ret);
// 			goto err2;
// 		svc_rdma_xdr_encode_write_list(rdma_resp, wr_lst, ret);
  	}
	 
	if (rp_ary) {
		ret = send_reply_chunks(rdma, rp_ary, rdma_resp, rqstp, vec);
// 	if (rp_ch) {
// 		ret = svc_rdma_send_reply_chunk(rdma, rp_ch, wr_lst, xdr);
  		if (ret < 0)
			goto err1;
		inline_bytes -= ret;
// 			goto err2;
// 		svc_rdma_xdr_encode_reply_chunk(rdma_resp, rp_ch, ret);
  	}
  
	 
  	ret = svc_rdma_post_recv(rdma, GFP_KERNEL);
  	if (ret)
  		goto err1;
	ret = send_reply(rdma, rqstp, res_page, rdma_resp, vec,
			 inline_bytes, inv_rkey);
// 	ret = svc_rdma_send_reply_msg(rdma, rdma_argp, rdma_resp, rqstp,
// 				      wr_lst, rp_ch);
  	if (ret < 0)
  		goto err0;
// 	return 0;
  
	svc_rdma_put_req_map(rdma, vec);
	dprintk("svcrdma: send_reply returns %d\n", ret);
	return ret;
//  err2:
// 	if (ret != -E2BIG)
// 		goto err1;
// 
// 	ret = svc_rdma_post_recv(rdma, GFP_KERNEL);
// 	if (ret)
// 		goto err1;
// 	ret = svc_rdma_send_error_msg(rdma, rdma_resp, rqstp);
// 	if (ret < 0)
// 		goto err0;
// 	return 0;
  
   err1:
  	put_page(res_page);
   err0:
	svc_rdma_put_req_map(rdma, vec);
  	pr_err("svcrdma: Could not send reply, err=%d. Closing transport.\n",
  	       ret);
	set_bit(XPT_CLOSE, &rdma->sc_xprt.xpt_flags);
// 	set_bit(XPT_CLOSE, &xprt->xpt_flags);
  	return -ENOTCONN;
  }