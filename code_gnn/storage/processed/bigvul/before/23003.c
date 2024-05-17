static int decode_attr_space_total(struct xdr_stream *xdr, uint32_t *bitmap, uint64_t *res)
{
	__be32 *p;
	int status = 0;

	*res = 0;
	if (unlikely(bitmap[1] & (FATTR4_WORD1_SPACE_TOTAL - 1U)))
		return -EIO;
	if (likely(bitmap[1] & FATTR4_WORD1_SPACE_TOTAL)) {
		READ_BUF(8);
		READ64(*res);
		bitmap[1] &= ~FATTR4_WORD1_SPACE_TOTAL;
	}
	dprintk("%s: space total=%Lu\n", __func__, (unsigned long long)*res);
	return status;
}