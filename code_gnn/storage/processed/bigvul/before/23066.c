static int encode_getfattr(struct xdr_stream *xdr, const u32* bitmask)
{
	return encode_getattr_two(xdr,
			bitmask[0] & nfs4_fattr_bitmap[0],
			bitmask[1] & nfs4_fattr_bitmap[1]);
}