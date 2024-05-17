static int decode_readlink(struct xdr_stream *xdr, struct rpc_rqst *req)
{
	struct xdr_buf *rcvbuf = &req->rq_rcv_buf;
	struct kvec *iov = rcvbuf->head;
	size_t hdrlen;
	u32 len, recvd;
	__be32 *p;
	char *kaddr;
	int status;

	status = decode_op_hdr(xdr, OP_READLINK);
	if (status)
		return status;

	 
	READ_BUF(4);
	READ32(len);
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
	 
	kaddr = (char *)kmap_atomic(rcvbuf->pages[0], KM_USER0);
	kaddr[len+rcvbuf->page_base] = '\0';
	kunmap_atomic(kaddr, KM_USER0);
	return 0;
}
