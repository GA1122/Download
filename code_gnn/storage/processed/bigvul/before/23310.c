static int decode_layoutget(struct xdr_stream *xdr, struct rpc_rqst *req,
			    struct nfs4_layoutget_res *res)
{
	__be32 *p;
	int status;
	u32 layout_count;
	struct xdr_buf *rcvbuf = &req->rq_rcv_buf;
	struct kvec *iov = rcvbuf->head;
	u32 hdrlen, recvd;

	status = decode_op_hdr(xdr, OP_LAYOUTGET);
	if (status)
		return status;
	p = xdr_inline_decode(xdr, 8 + NFS4_STATEID_SIZE);
	if (unlikely(!p))
		goto out_overflow;
	res->return_on_close = be32_to_cpup(p++);
	p = xdr_decode_opaque_fixed(p, res->stateid.data, NFS4_STATEID_SIZE);
	layout_count = be32_to_cpup(p);
	if (!layout_count) {
		dprintk("%s: server responded with empty layout array\n",
			__func__);
		return -EINVAL;
	}

	p = xdr_inline_decode(xdr, 28);
	if (unlikely(!p))
		goto out_overflow;
	p = xdr_decode_hyper(p, &res->range.offset);
	p = xdr_decode_hyper(p, &res->range.length);
	res->range.iomode = be32_to_cpup(p++);
	res->type = be32_to_cpup(p++);
	res->layoutp->len = be32_to_cpup(p);

	dprintk("%s roff:%lu rlen:%lu riomode:%d, lo_type:0x%x, lo.len:%d\n",
		__func__,
		(unsigned long)res->range.offset,
		(unsigned long)res->range.length,
		res->range.iomode,
		res->type,
		res->layoutp->len);

	hdrlen = (u8 *) xdr->p - (u8 *) iov->iov_base;
	recvd = req->rq_rcv_buf.len - hdrlen;
	if (res->layoutp->len > recvd) {
		dprintk("NFS: server cheating in layoutget reply: "
				"layout len %u > recvd %u\n",
				res->layoutp->len, recvd);
		return -EINVAL;
	}

	xdr_read_pages(xdr, res->layoutp->len);

	if (layout_count > 1) {
		 
		dprintk("%s: server responded with %d layouts, dropping tail\n",
			__func__, layout_count);
	}

	return 0;
out_overflow:
	print_overflow_msg(__func__, xdr);
	return -EIO;
}