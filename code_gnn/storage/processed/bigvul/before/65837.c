static __be32 nfsd4_encode_splice_read(
				struct nfsd4_compoundres *resp,
				struct nfsd4_read *read,
				struct file *file, unsigned long maxcount)
{
	struct xdr_stream *xdr = &resp->xdr;
	struct xdr_buf *buf = xdr->buf;
	u32 eof;
	long len;
	int space_left;
	__be32 nfserr;
	__be32 *p = xdr->p - 2;

	 
	if (xdr->end - xdr->p < 1)
		return nfserr_resource;

	len = maxcount;
	nfserr = nfsd_splice_read(read->rd_rqstp, file,
				  read->rd_offset, &maxcount);
	if (nfserr) {
		 
		buf->page_len = 0;
		return nfserr;
	}

	eof = nfsd_eof_on_read(len, maxcount, read->rd_offset,
				d_inode(read->rd_fhp->fh_dentry)->i_size);

	*(p++) = htonl(eof);
	*(p++) = htonl(maxcount);

	buf->page_len = maxcount;
	buf->len += maxcount;
	xdr->page_ptr += (buf->page_base + maxcount + PAGE_SIZE - 1)
							/ PAGE_SIZE;

	 
	buf->tail[0].iov_base = xdr->p;
	buf->tail[0].iov_len = 0;
	xdr->iov = buf->tail;
	if (maxcount&3) {
		int pad = 4 - (maxcount&3);

		*(xdr->p++) = 0;

		buf->tail[0].iov_base += maxcount&3;
		buf->tail[0].iov_len = pad;
		buf->len += pad;
	}

	space_left = min_t(int, (void *)xdr->end - (void *)xdr->p,
				buf->buflen - buf->len);
	buf->buflen = buf->len + space_left;
	xdr->end = (__be32 *)((void *)xdr->end + space_left);

	return 0;
}