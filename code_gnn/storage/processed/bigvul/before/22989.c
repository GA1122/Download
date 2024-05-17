static int decode_attr_link_support(struct xdr_stream *xdr, uint32_t *bitmap, uint32_t *res)
{
	__be32 *p;

	*res = 0;
	if (unlikely(bitmap[0] & (FATTR4_WORD0_LINK_SUPPORT - 1U)))
		return -EIO;
	if (likely(bitmap[0] & FATTR4_WORD0_LINK_SUPPORT)) {
		READ_BUF(4);
		READ32(*res);
		bitmap[0] &= ~FATTR4_WORD0_LINK_SUPPORT;
	}
	dprintk("%s: link support=%s\n", __func__, *res == 0 ? "false" : "true");
	return 0;
}