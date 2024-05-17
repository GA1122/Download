static int decode_readlink(struct xdr_stream *xdr, struct rpc_rqst *req)
{
	struct xdr_buf *rcvbuf = &req->rq_rcv_buf;
	struct kvec *iov = rcvbuf->head;
	size_t hdrlen;
	u32 len, recvd;
	__be32 *p;
	int status;

	status = decode_op_hdr(xdr, OP_READLINK);
	if (status)
		return status;

	 
	p = xdr_inline_decode(xdr, 4);
	if (unlikely(!p))
		goto out_overflow;
	len = be32_to_cpup(p);
	if (len >= rcvbuf->page_len || len <= 0) {
		dprintk("nfs: server returned giant symlink!\n");
		return -ENAMETOOLONG;
	}
	hdrlen = (char *) xdr->p - (char *) iov->iov_base;
	recvd = req->rq_rcv_buf.len - hdrlen;
	if (recvd < len) {
		dprintk("NFS: server cheating in readlink reply: "
				"count %u > recvd %u\n", len, recvd);
		return -EIO;
	}
	xdr_read_pages(xdr, len);
	 
	xdr_terminate_string(rcvbuf, len);
	return 0;
out_overflow:
	print_overflow_msg(__func__, xdr);
	return -EIO;
}