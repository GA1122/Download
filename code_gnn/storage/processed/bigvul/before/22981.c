static int decode_attr_files_avail(struct xdr_stream *xdr, uint32_t *bitmap, uint64_t *res)
{
	__be32 *p;
	int status = 0;

	*res = 0;
	if (unlikely(bitmap[0] & (FATTR4_WORD0_FILES_AVAIL - 1U)))
		return -EIO;
	if (likely(bitmap[0] & FATTR4_WORD0_FILES_AVAIL)) {
		READ_BUF(8);
		READ64(*res);
		bitmap[0] &= ~FATTR4_WORD0_FILES_AVAIL;
	}
	dprintk("%s: files avail=%Lu\n", __func__, (unsigned long long)*res);
	return status;
}