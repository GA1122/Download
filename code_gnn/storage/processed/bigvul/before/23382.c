static void encode_readdir(struct xdr_stream *xdr, const struct nfs4_readdir_arg *readdir, struct rpc_rqst *req, struct compound_hdr *hdr)
{
	uint32_t attrs[2] = {
		FATTR4_WORD0_RDATTR_ERROR,
		FATTR4_WORD1_MOUNTED_ON_FILEID,
	};
	uint32_t dircount = readdir->count >> 1;
	__be32 *p;

	if (readdir->plus) {
		attrs[0] |= FATTR4_WORD0_TYPE|FATTR4_WORD0_CHANGE|FATTR4_WORD0_SIZE|
			FATTR4_WORD0_FSID|FATTR4_WORD0_FILEHANDLE|FATTR4_WORD0_FILEID;
		attrs[1] |= FATTR4_WORD1_MODE|FATTR4_WORD1_NUMLINKS|FATTR4_WORD1_OWNER|
			FATTR4_WORD1_OWNER_GROUP|FATTR4_WORD1_RAWDEV|
			FATTR4_WORD1_SPACE_USED|FATTR4_WORD1_TIME_ACCESS|
			FATTR4_WORD1_TIME_METADATA|FATTR4_WORD1_TIME_MODIFY;
		dircount >>= 1;
	}
	 
	if (!(readdir->bitmask[1] & FATTR4_WORD1_MOUNTED_ON_FILEID))
		attrs[0] |= FATTR4_WORD0_FILEID;

	p = reserve_space(xdr, 12+NFS4_VERIFIER_SIZE+20);
	*p++ = cpu_to_be32(OP_READDIR);
	p = xdr_encode_hyper(p, readdir->cookie);
	p = xdr_encode_opaque_fixed(p, readdir->verifier.data, NFS4_VERIFIER_SIZE);
	*p++ = cpu_to_be32(dircount);
	*p++ = cpu_to_be32(readdir->count);
	*p++ = cpu_to_be32(2);

	*p++ = cpu_to_be32(attrs[0] & readdir->bitmask[0]);
	*p = cpu_to_be32(attrs[1] & readdir->bitmask[1]);
	hdr->nops++;
	hdr->replen += decode_readdir_maxsz;
	dprintk("%s: cookie = %Lu, verifier = %08x:%08x, bitmap = %08x:%08x\n",
			__func__,
			(unsigned long long)readdir->cookie,
			((u32 *)readdir->verifier.data)[0],
			((u32 *)readdir->verifier.data)[1],
			attrs[0] & readdir->bitmask[0],
			attrs[1] & readdir->bitmask[1]);
}