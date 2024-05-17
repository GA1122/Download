static inline u32 nfsd4_exchange_id_rsize(struct svc_rqst *rqstp, struct nfsd4_op *op)
{
	return (op_encode_hdr_size + 2 + 1 +  \
		1 + 1 +  \
		4 +  \
		2 +  \
		 \
		XDR_QUADLEN(NFS4_OPAQUE_LIMIT) + 1 +\
		 \
		XDR_QUADLEN(NFS4_OPAQUE_LIMIT) + 1 +\
		1 +  \
		0  ) * sizeof(__be32);
}
