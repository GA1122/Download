static int decode_readdir(struct xdr_stream *xdr, struct rpc_rqst *req, struct nfs4_readdir_res *readdir)
{
	struct xdr_buf	*rcvbuf = &req->rq_rcv_buf;
	struct page	*page = *rcvbuf->pages;
	struct kvec	*iov = rcvbuf->head;
	size_t		hdrlen;
	u32		recvd, pglen = rcvbuf->page_len;
	__be32		*end, *entry, *p, *kaddr;
	unsigned int	nr = 0;
	int		status;

	status = decode_op_hdr(xdr, OP_READDIR);
	if (status)
		return status;
	READ_BUF(8);
	COPYMEM(readdir->verifier.data, 8);
	dprintk("%s: verifier = %08x:%08x\n",
			__func__,
			((u32 *)readdir->verifier.data)[0],
			((u32 *)readdir->verifier.data)[1]);


	hdrlen = (char *) p - (char *) iov->iov_base;
	recvd = rcvbuf->len - hdrlen;
	if (pglen > recvd)
		pglen = recvd;
	xdr_read_pages(xdr, pglen);

	BUG_ON(pglen + readdir->pgbase > PAGE_CACHE_SIZE);
	kaddr = p = kmap_atomic(page, KM_USER0);
	end = p + ((pglen + readdir->pgbase) >> 2);
	entry = p;

	 
	if ((entry + 1) > end)
		goto short_pkt;

	for (; *p++; nr++) {
		u32 len, attrlen, xlen;
		if (end - p < 3)
			goto short_pkt;
		dprintk("cookie = %Lu, ", *((unsigned long long *)p));
		p += 2;			 
		len = ntohl(*p++);	 
		if (len > NFS4_MAXNAMLEN) {
			dprintk("NFS: giant filename in readdir (len 0x%x)\n",
					len);
			goto err_unmap;
		}
		xlen = XDR_QUADLEN(len);
		if (end - p < xlen + 1)
			goto short_pkt;
		dprintk("filename = %*s\n", len, (char *)p);
		p += xlen;
		len = ntohl(*p++);	 
		if (end - p < len + 1)
			goto short_pkt;
		p += len;
		attrlen = XDR_QUADLEN(ntohl(*p++));
		if (end - p < attrlen + 2)
			goto short_pkt;
		p += attrlen;		 
		entry = p;
	}
	 
	if (!nr && entry[1] == 0) {
		dprintk("NFS: readdir reply truncated!\n");
		entry[1] = 1;
	}
out:	
	kunmap_atomic(kaddr, KM_USER0);
	return 0;
short_pkt:
	 
	dprintk("%s: short packet at entry %d\n", __func__, nr);
	entry[0] = entry[1] = 0;
	if (nr)
		goto out;
err_unmap:
	kunmap_atomic(kaddr, KM_USER0);
	return -errno_NFSERR_IO;
}
