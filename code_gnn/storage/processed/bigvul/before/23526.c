void xdr_enter_page(struct xdr_stream *xdr, unsigned int len)
{
	xdr_read_pages(xdr, len);
	 
	xdr_set_page_base(xdr, 0, len);
}
