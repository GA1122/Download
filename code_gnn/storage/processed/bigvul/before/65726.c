nfs4svc_encode_compoundres(struct svc_rqst *rqstp, __be32 *p, struct nfsd4_compoundres *resp)
{
	 
	struct xdr_buf *buf = resp->xdr.buf;

	WARN_ON_ONCE(buf->len != buf->head[0].iov_len + buf->page_len +
				 buf->tail[0].iov_len);

	rqstp->rq_next_page = resp->xdr.page_ptr + 1;

	p = resp->tagp;
	*p++ = htonl(resp->taglen);
	memcpy(p, resp->tag, resp->taglen);
	p += XDR_QUADLEN(resp->taglen);
	*p++ = htonl(resp->opcnt);

	nfsd4_sequence_done(resp);
	return 1;
}